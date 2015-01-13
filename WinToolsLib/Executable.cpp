#include "Executable.h"
#include "IStream.h"
#include "Log.h"
#include <Windows.h>
#include <algorithm>

namespace WinToolsLib
{
	DosHeader::DosHeader() :
		m_ntHeadersOffset(0)
	{
	}

	Void DosHeader::Parse(IStream& stream)
	{
		UInt16 magic = 0;
		stream >> magic;
		if (IMAGE_DOS_SIGNATURE != magic)
		{
			THROW_BAD_IMAGE_FORMAT_EXCEPTION();
		}
		stream.Skip(58);
		stream >> m_ntHeadersOffset;
	}

	FileHeader::FileHeader() :
		m_machine(0),
		m_numberOfSections(0),
		m_timeDateStamp(0),
		m_pointerToSymbolTable(0),
		m_numberOfSymbols(0),
		m_sizeOfOptionalHeader(0),
		m_characteristics(0)
	{
	}

	Void FileHeader::Parse(IStream& stream)
	{
		stream >> m_machine;
		stream >> m_numberOfSections;
		stream >> m_timeDateStamp;
		stream >> m_pointerToSymbolTable;
		stream >> m_numberOfSymbols;
		stream >> m_sizeOfOptionalHeader;
		stream >> m_characteristics;
	}

	DataDirectory::DataDirectory() :
		m_virtualAddress(0),
		m_size(0)
	{
	}

	Void DataDirectory::Parse(IStream& stream)
	{
		stream >> m_virtualAddress;
		stream >> m_size;
	}

	OptionalHeader::OptionalHeader() :
		m_is32bit(True),
		m_magic(0),
		m_majorLinkerVersion(0),
		m_minorLinkerVersion(0),
		m_sizeOfCode(0),
		m_sizeOfInitializedData(0),
		m_sizeOfUninitializedData(0),
		m_addressOfEntryPoint(0),
		m_baseOfCode(0),
		m_baseOfData(0),
		m_imageBase32(0),
		m_sectionAlignment(0),
		m_fileAlignment(0),
		m_majorOperatingSystemVersion(0),
		m_minorOperatingSystemVersion(0),
		m_majorImageVersion(0),
		m_minorImageVersion(0),
		m_majorSubsystemVersion(0),
		m_minorSubsystemVersion(0),
		m_win32VersionValue(0),
		m_sizeOfImage(0),
		m_sizeOfHeaders(0),
		m_checkSum(0),
		m_subsystem(0),
		m_dllCharacteristics(0),
		m_sizeOfStackReserve64(0),
		m_sizeOfStackCommit64(0),
		m_sizeOfHeapReserve64(0),
		m_sizeOfHeapCommit64(0),
		m_loaderFlags(0)
	{
	}

	Void OptionalHeader::Parse(IStream& stream)
	{
		stream >> m_magic;

		if (IMAGE_NT_OPTIONAL_HDR32_MAGIC == m_magic)
		{
			m_is32bit = True;
		}
		else if (IMAGE_NT_OPTIONAL_HDR64_MAGIC == m_magic)
		{
			m_is32bit = False;
		}
		else
		{
			THROW_BAD_IMAGE_FORMAT_EXCEPTION();
		}

		stream >> m_majorLinkerVersion;
		stream >> m_minorLinkerVersion;
		stream >> m_sizeOfCode;
		stream >> m_sizeOfInitializedData;
		stream >> m_sizeOfUninitializedData;
		stream >> m_addressOfEntryPoint;
		stream >> m_baseOfCode;

		if (m_is32bit)
		{
			stream >> m_baseOfData;
			stream >> m_imageBase32;
		}
		else
		{
			stream >> m_imageBase64;
		}

		stream >> m_sectionAlignment;
		stream >> m_fileAlignment;
		stream >> m_majorOperatingSystemVersion;
		stream >> m_minorOperatingSystemVersion;
		stream >> m_majorImageVersion;
		stream >> m_minorImageVersion;
		stream >> m_majorSubsystemVersion;
		stream >> m_minorSubsystemVersion;
		stream >> m_win32VersionValue;
		stream >> m_sizeOfImage;
		stream >> m_sizeOfHeaders;
		stream >> m_checkSum;
		stream >> m_subsystem;
		stream >> m_dllCharacteristics;

		if (m_is32bit)
		{
			stream >> m_sizeOfStackReserve32;
			stream >> m_sizeOfStackCommit32;
			stream >> m_sizeOfHeapReserve32;
			stream >> m_sizeOfHeapCommit32;
		}
		else
		{
			stream >> m_sizeOfStackReserve64;
			stream >> m_sizeOfStackCommit64;
			stream >> m_sizeOfHeapReserve64;
			stream >> m_sizeOfHeapCommit64;
		}

		stream >> m_loaderFlags;

		UInt32 numberOfDirectories = 0;
		stream >> numberOfDirectories;
		m_directories.resize(numberOfDirectories);

		std::for_each(
			begin(m_directories),
			end(m_directories),
			[&stream](DataDirectory& directory)
			{
				directory.Parse(stream);
			});
	}

	UInt64 OptionalHeader::GetImageBase() const
	{
		if (m_is32bit)
		{
			return m_imageBase32;
		}
		return m_imageBase64;
	}

	NtHeaders::NtHeaders()
	{
	}

	Void NtHeaders::Parse(IStream& stream)
	{
		UInt32 signature = 0;
		stream >> signature;

		if (IMAGE_NT_SIGNATURE != signature)
		{
			THROW_BAD_IMAGE_FORMAT_EXCEPTION();
		}
		
		m_fileHeader.Parse(stream);
		m_optionalHeader.Parse(stream);
	}

	SectionHeader::SectionHeader() :
		m_virtualSize(0),
		m_virtualAddress(0),
		m_sizeOfRawData(0),
		m_pointerToRawData(0),
		m_pointerToRelocations(0),
		m_pointerToLineNumbers(0),
		m_numberOfRelocations(0),
		m_numberOfLineNumbers(0),
		m_characteristics(0)
	{
	}

	Void SectionHeader::Parse(IStream& stream)
	{
		Char name[9] = {0};
		stream >> *((PUInt64)name);
		m_name = name;

		stream >> m_virtualSize;
		stream >> m_virtualAddress;
		stream >> m_sizeOfRawData;
		stream >> m_pointerToRawData;
		stream >> m_pointerToRelocations;
		stream >> m_pointerToLineNumbers;
		stream >> m_numberOfRelocations;
		stream >> m_numberOfLineNumbers;
		stream >> m_characteristics;
	}

	Bool SectionHeader::IsExecutable() const
	{
		if (m_characteristics & IMAGE_SCN_MEM_EXECUTE)
		{
			return True;
		}
		return False;
	}

	Bool SectionHeader::IsWriteable() const
	{
		if (m_characteristics & IMAGE_SCN_MEM_WRITE)
		{
			return True;
		}
		return False;
	}

	Bool SectionHeader::IsDiscardable() const
	{
		if (m_characteristics & IMAGE_SCN_MEM_DISCARDABLE)
		{
			return True;
		}
		return False;
	}

	RelocationBlock::RelocationBlock() :
		m_virtualAddress(0),
		m_size(0)
	{
	}

	Void RelocationBlock::Parse(IStream& stream)
	{
		stream >> m_virtualAddress;
		stream >> m_size;

		SizeT count = (m_size - 2 * sizeof(UInt32)) / sizeof(UInt16);
		m_relocations.resize(count);

		std::for_each(
			begin(m_relocations),
			end(m_relocations),
			[&stream](Relocation& reloc)
			{
				stream >> reloc.m_raw;
			});
	}

	ExportDirectory::ExportDirectory() :
		m_characteristics(0),
		m_timeDateStamp(0),
		m_majorVersion(0),
		m_minorVersion(0),
		m_nameRva(0),
		m_ordinalBase(0),
		m_numberOfFunctions(0),
		m_numberOfNames(0),
		m_addressOfFunctions(0),
		m_addressOfNames(0),
		m_addressOfNameOrdinals(0)
	{
	}

	Void ExportDirectory::Parse(IStream& stream)
	{
		stream >> m_characteristics;
		stream >> m_timeDateStamp;
		stream >> m_majorVersion;
		stream >> m_minorVersion;
		stream >> m_nameRva;
		stream >> m_ordinalBase;
		stream >> m_numberOfFunctions;
		stream >> m_numberOfNames;
		stream >> m_addressOfFunctions;
		stream >> m_addressOfNames;
		stream >> m_addressOfNameOrdinals;
	}

	ExportSymbol::ExportSymbol() :
		m_rva(0),
		m_byOrdinal(False),
		m_ordinal(0),
		m_isForwarded(False),
		m_targetByOrdinal(False),
		m_targetOrdinal(0)
	{
	}

	ExportSymbol::ExportSymbol(UInt32 rva, UInt16 ordinal) :
		m_rva(rva),
		m_byOrdinal(True),
		m_ordinal(ordinal),
		m_isForwarded(False),
		m_targetByOrdinal(False),
		m_targetOrdinal(0)
	{
	}

	ExportSymbol::ExportSymbol(UInt32 rva, UInt16 ordinal, StringA name) :
		m_rva(rva),
		m_byOrdinal(False),
		m_ordinal(ordinal),
		m_name(std::move(name)),
		m_isForwarded(False),
		m_targetByOrdinal(False),
		m_targetOrdinal(0)
	{
	}
	
	ExportSymbol::ExportSymbol(UInt32 rva, UInt16 ordinal, StringA&& targetModule, UInt16 targetOrdinal) :
		m_rva(rva),
		m_byOrdinal(True),
		m_ordinal(ordinal),
		m_isForwarded(True),
		m_targetModule(std::move(targetModule)),
		m_targetByOrdinal(True),
		m_targetOrdinal(targetOrdinal)
	{

	}
	
	ExportSymbol::ExportSymbol(UInt32 rva, UInt16 ordinal, StringA&& targetModule, StringA&& targetName) :
		m_rva(rva),
		m_byOrdinal(True),
		m_ordinal(ordinal),
		m_isForwarded(True),
		m_targetModule(std::move(targetModule)),
		m_targetByOrdinal(False),
		m_targetName(std::move(targetName))
	{
	}

	ExportSymbol::ExportSymbol(UInt32 rva, UInt16 ordinal, StringA name, StringA&& targetModule, UInt16 targetOrdinal) :
		m_rva(rva),
		m_byOrdinal(False),
		m_ordinal(ordinal),
		m_name(std::move(name)),
		m_isForwarded(True),
		m_targetModule(std::move(targetModule)),
		m_targetByOrdinal(True),
		m_targetOrdinal(targetOrdinal)
	{
	}

	ExportSymbol::ExportSymbol(UInt32 rva, UInt16 ordinal, StringA name, StringA&& targetModule, StringA&& targetName) :
		m_rva(rva),
		m_byOrdinal(False),
		m_ordinal(ordinal),
		m_name(std::move(name)),
		m_isForwarded(True),
		m_targetModule(std::move(targetModule)),
		m_targetByOrdinal(False),
		m_targetName(std::move(targetName))
	{
	}

	ExportSymbol::ExportSymbol(const ExportSymbol& other) :
		m_rva(other.m_rva),
		m_byOrdinal(other.m_byOrdinal),
		m_ordinal(other.m_ordinal),
		m_name(other.m_name),
		m_isForwarded(other.m_isForwarded),
		m_targetModule(other.m_targetModule),
		m_targetByOrdinal(other.m_targetByOrdinal),
		m_targetOrdinal(other.m_targetOrdinal),
		m_targetName(other.m_targetName)
	{
	}

	ExportSymbol::ExportSymbol(ExportSymbol&& other) :
		m_rva(other.m_rva),
		m_byOrdinal(other.m_byOrdinal),
		m_ordinal(other.m_ordinal),
		m_name(std::move(other.m_name)),
		m_isForwarded(other.m_isForwarded),
		m_targetModule(std::move(other.m_targetModule)),
		m_targetByOrdinal(other.m_targetByOrdinal),
		m_targetOrdinal(other.m_targetOrdinal),
		m_targetName(std::move(other.m_targetName))
	{
	}

	Bool ExportSymbol::IsNull() const
	{
		if (0 == m_rva)
		{
			return True;
		}
		return False;
	}

	StringA ExportSymbol::GetName() const
	{
		if (m_byOrdinal)
		{
			StringA name;
			name.Format("[%u]", m_ordinal);
			return name;
		}
		return m_name;
	}

	Export::Export()
	{
	}

	Export::Export(const Export& other) :
		m_name(other.m_name),
		m_directory(other.m_directory),
		m_ordinalTable(other.m_ordinalTable),
		m_nameTable(other.m_nameTable),
		m_addressTable(other.m_addressTable),
		m_rva2Symbol(other.m_rva2Symbol),
		m_name2Symbol(other.m_name2Symbol),
		m_ordinal2Symbol(other.m_ordinal2Symbol)
	{
	}

	Export::Export(Export&& other) :
		m_name(std::move(other.m_name)),
		m_directory(other.m_directory),
		m_ordinalTable(std::move(other.m_ordinalTable)),
		m_nameTable(std::move(other.m_nameTable)),
		m_addressTable(std::move(other.m_addressTable)),
		m_rva2Symbol(std::move(other.m_rva2Symbol)),
		m_name2Symbol(std::move(other.m_name2Symbol)),
		m_ordinal2Symbol(std::move(other.m_ordinal2Symbol))
	{
	}

	const ExportSymbol* Export::GetSymbol(UInt16 ordinal) const
	{
		auto found = m_ordinal2Symbol.find(ordinal);
		if (found != std::end(m_ordinal2Symbol))
		{
			return found->second.get();
		}
		return nullptr;
	}
	
	const ExportSymbol* Export::GetSymbol(const StringA& name) const
	{
		auto found = m_name2Symbol.find(name);
		if (found != std::end(m_name2Symbol))
		{
			return found->second.get();
		}
		return nullptr;
	}

	const ExportSymbol* Export::GetNearestSymbol(UInt32 rva, UInt32& offset) const
	{
		auto found = m_rva2Symbol.find(rva);
		if (found != std::end(m_rva2Symbol))
		{
			offset = 0;
			return found->second.get();
		}
		auto nearest = m_rva2Symbol.upper_bound(rva);
		if (nearest != std::end(m_rva2Symbol) &&
			nearest != std::begin(m_rva2Symbol))
		{
			nearest--;
			offset = rva - nearest->second->m_rva;
			return nearest->second.get();
		}
		return nullptr;
	}

	StringA Export::GetNearestSymbolName(UInt32 rva) const
	{
		UInt32 offset = 0;

		auto symbol = GetNearestSymbol(rva, offset);
		if (symbol)
		{
			if (offset > 0)
			{
				StringA name;
				name.Format("%s + 0x%X", symbol->GetName().GetBuffer(), offset);
				return name;
			}
			
			return symbol->GetName();
		}

		StringA strRva;
		strRva.Format("0x%X", rva);
		return strRva;
	}

	ExportRva2SymbolMap Export::GetSymbolMap(UInt32 rva, UInt32 size) const
	{
		typedef std::pair<ExportRva2SymbolMap::key_type, ExportRva2SymbolMap::mapped_type> ExportRva2SymbolMapPair;
		
		ExportRva2SymbolMap map;
		UInt32 end = rva + size;

		std::for_each(
			std::begin(m_rva2Symbol),
			std::end(m_rva2Symbol),
			[&map, rva, end](const ExportRva2SymbolMapPair& pair)
		{
			if (pair.first >= rva && pair.first < end)
			{
				map[pair.first] = pair.second;
			}
		});

		return map;
	}

	ImportDescriptor::ImportDescriptor() :
		m_nameTableRva(0),
		m_timeDateStamp(0),
		m_forwarderChain(0),
		m_nameRva(0),
		m_addressTableRva(0)
	{
	}

	Void ImportDescriptor::Parse(IStream& stream)
	{
		stream >> m_nameTableRva;
		stream >> m_timeDateStamp;
		stream >> m_forwarderChain;
		stream >> m_nameRva;
		stream >> m_addressTableRva;
	}

	Bool ImportDescriptor::IsNull() const
	{
		if (0 == m_nameTableRva &&
			0 == m_timeDateStamp &&
			0 == m_forwarderChain &&
			0 == m_nameRva &&
			0 == m_addressTableRva)
		{
			return True;
		}
		return False;
	}

	ImportSymbol::ImportSymbol() :
		m_byOrdinal(False),
		m_ordinal(0)
	{
	}

	ImportSymbol::ImportSymbol(UInt16 ordinal) :
		m_byOrdinal(True),
		m_ordinal(ordinal)
	{
	}

	ImportSymbol::ImportSymbol(StringA&& name) :
		m_byOrdinal(False),
		m_name(std::move(name))
	{
	}

	ImportSymbol::ImportSymbol(const ImportSymbol& other) :
		m_byOrdinal(other.m_byOrdinal),
		m_ordinal(other.m_ordinal),
		m_name(other.m_name)
	{
	}

	ImportSymbol::ImportSymbol(ImportSymbol&& other) :
		m_byOrdinal(other.m_byOrdinal),
		m_ordinal(other.m_ordinal),
		m_name(std::move(other.m_name))
	{
	}

	StringA ImportSymbol::GetName() const
	{
		if (m_byOrdinal)
		{
			StringA name;
			name.Format("[%u]", m_ordinal);
			return name;
		}
		return m_name;
	}

	Import::Import()
	{
	}

	Import::Import(const Import& other) :
		m_descriptor(other.m_descriptor),
		m_nameTable(other.m_nameTable),
		m_addressTable(other.m_addressTable),
		m_name(other.m_name),
		m_symbols(other.m_symbols)
	{
	}

	Import::Import(Import&& other) :
		m_descriptor(other.m_descriptor),
		m_nameTable(std::move(other.m_nameTable)),
		m_addressTable(std::move(other.m_addressTable)),
		m_name(std::move(other.m_name)),
		m_symbols(std::move(other.m_symbols))
	{
	}

	DelayImportDescriptor::DelayImportDescriptor() :
		m_attributes(0),
		m_dllNameRva(0),
		m_hModuleRva(0),
		m_addressTableRva(0),
		m_nameTableRva(0),
		m_boundIatRva(0),
		m_unloadIatRva(0),
		m_timeDateStamp(0)
	{
	}

	Void DelayImportDescriptor::Parse(IStream& stream)
	{
		stream >> m_attributes;
		stream >> m_dllNameRva;
		stream >> m_hModuleRva;
		stream >> m_addressTableRva;
		stream >> m_nameTableRva;
		stream >> m_boundIatRva;
		stream >> m_unloadIatRva;
		stream >> m_timeDateStamp;
	}

	Bool DelayImportDescriptor::IsNull() const
	{
		if (0 == m_attributes &&
			0 == m_dllNameRva &&
			0 == m_hModuleRva &&
			0 == m_addressTableRva &&
			0 == m_nameTableRva &&
			0 == m_boundIatRva &&
			0 == m_unloadIatRva &&
			0 == m_timeDateStamp)
		{
			return True;
		}
		return False;
	}

	DebugDirectory::DebugDirectory() :
		m_characteristics(0),
		m_timeDateStamp(0),
		m_majorVersion(0),
		m_minorVersion(0),
		m_type(0),
		m_sizeOfData(0),
		m_addressOfRawData(0),
		m_pointerToRawData(0)
	{
	}

	Void DebugDirectory::Parse(IStream& stream)
	{
		stream >> m_characteristics;
		stream >> m_timeDateStamp;
		stream >> m_majorVersion;
		stream >> m_minorVersion;
		stream >> m_type;
		stream >> m_sizeOfData;
		stream >> m_addressOfRawData;
		stream >> m_pointerToRawData;
	}

	Executable::Executable(Bool isKernel) :
		m_isKernel(isKernel)
	{
	}

	Void Executable::Parse(IStream& stream)
	{
		m_codeChunks.Clear();
		m_dataChunks.Clear();

		stream.Reset();
		m_dosHeader.Parse(stream);

		AddDataChunk(0, stream.GetPosition(), 0, "DOS Header");

		stream.SetPosition(m_dosHeader.m_ntHeadersOffset);
		m_ntHeaders.Parse(stream);

		m_sections.resize(m_ntHeaders.m_fileHeader.m_numberOfSections);
		std::for_each(
			begin(m_sections),
			end(m_sections),
			[this, &stream](SectionHeader& section)
			{
				section.Parse(stream);
				if (section.IsExecutable())
				{
					if(m_isKernel==False || (section.IsDiscardable()==False && section.IsWriteable()==False))
					{
						auto size = min(section.m_sizeOfRawData, section.m_virtualSize);
						AddCodeChunk(
							section.m_pointerToRawData,
							section.m_pointerToRawData + size,
							section.m_virtualAddress,
							section.m_name.GetBuffer());
					} 
				}
			});
		
		AddDataChunk(
			m_dosHeader.m_ntHeadersOffset,
			stream.GetPosition(),
			m_dosHeader.m_ntHeadersOffset,
			"NT Headers");

		ParseExportDirectory(stream);
		ParseImportDirectory(stream);
		ParseResourceDirectory(stream);
		ParseExceptionDirectory(stream);
		ParseSecurityDirectory(stream);
		ParseBaseRelocationTable(stream);
		ParseDebugDirectory(stream);
		ParseArchitectureSpecificData(stream);
		ParseGlobalPointerTable(stream);
		ParseThreadLocalStorageDirectory(stream);
		ParseLoadConfigurationDirectory(stream);
		ParseBoundImportDirectory(stream);
		ParseImportAddressTable(stream);
		ParseDelayImportDescriptors(stream);
		ParseComRuntimeDescriptor(stream);
	}

	Void Executable::Relocate(IStream& stream, PByte newBase)
	{
		auto index = (SizeT)DataDirectory::BaseRelocationTable;
		auto relocTable = m_ntHeaders.m_optionalHeader.m_directories[index];
		
		if (0 == relocTable.m_virtualAddress)
		{
			return; // Image doesn't contain relocation table
		}
		
		auto offset = RvaToOffset(relocTable.m_virtualAddress);
		stream.SetPosition(offset);

		RelocationBlockArray relocationBlocks;

		while (relocTable.m_size)
		{
			RelocationBlock block;
			block.Parse(stream);

			relocationBlocks.push_back(std::move(block));
			relocTable.m_size -= block.m_size;
		}

		auto diff = (SizeT)newBase - (SizeT)m_ntHeaders.m_optionalHeader.GetImageBase();

		for (auto i = begin(relocationBlocks); i != end(relocationBlocks); i++)
		{
			const RelocationBlock& block = *i;
			
			for (auto j = begin(block.m_relocations); j != end(block.m_relocations); j++)
			{
				const Relocation& reloc = *j;

				auto offset = RvaToOffset(block.m_virtualAddress + reloc.m_offset);
				stream.SetPosition(offset);

				switch (reloc.m_type)
				{
					case IMAGE_REL_BASED_HIGHLOW:
					{
						UInt32 fixup;
						stream >> fixup;
						fixup += (UInt32)diff;
						stream.SetPosition(offset);
						stream << fixup;
						break;
					}

					case IMAGE_REL_BASED_DIR64:
					{
						UInt64 fixup;
						stream >> fixup;
						fixup += (UInt64)diff;
						stream.SetPosition(offset);
						stream << fixup;
						break;
					}

					case IMAGE_REL_BASED_ABSOLUTE:
					{
						// No fixup required
						break;
					}

					default:
					{
						THROW_UNSUPPORTED_RELOCATION_TYPE_EXCEPTION();
					}
				}
			}
		}
	}

	Void Executable::AddDataChunk(UInt32 rva, SizeT size, PCChar name)
	{
		auto begin = RvaToOffset(rva);
		auto end = begin + size;
		AddDataChunk(begin, end, rva, name);
	}
	
	Void Executable::AnalyzeChunks()
	{
		m_codeChunks.Normalize();
		m_dataChunks.Normalize();

		m_codeChunks -= m_dataChunks;
	}

	StringA Executable::ReadString(IStream& stream) const
	{
		std::string result;

		Int8 a = ' ';
		while (a != '\0')
		{
			stream >> a;
			result.push_back(a);
		}

		return StringA(result.c_str());
	}

	StringA Executable::ReadString(IStream& stream, SizeT rva) const
	{
		auto offset = RvaToOffset(rva);
		stream.SetPosition(offset);

		return ReadString(stream);
	}

	StringA Executable::GetExportName(IStream& stream, UInt16 ordinal) const
	{
		if (0 == m_export.m_directory.m_addressOfNames ||
			0 == m_export.m_directory.m_addressOfNameOrdinals)
		{
			return StringA(); // Image doesn't contain named exports
		}

		for (UInt32 i = 0; i < m_export.m_directory.m_numberOfNames; i++)
		{
			if (m_export.m_ordinalTable[i] == ordinal)
			{
				return ReadString(stream, m_export.m_nameTable[i]);
			}
		}

		return StringA::Convert(ordinal + m_export.m_directory.m_ordinalBase);
	}

	UInt32 Executable::GetExportAddress(IStream& stream, StringA name) const
	{
		if (0 == m_export.m_directory.m_addressOfNames ||
			0 == m_export.m_directory.m_addressOfNameOrdinals ||
			0 == m_export.m_directory.m_addressOfFunctions )
		{
			return NULL; // Image doesn't contain named exports
		}

		for (UInt32 i = 0; i < m_export.m_directory.m_numberOfNames; i++)
		{
			if(ReadString(stream, m_export.m_nameTable[i]).Find(name)==0)
			{
				return m_export.m_addressTable[m_export.m_ordinalTable[i]];
			}
		}

		return 0;
	}

	Bool Executable::Is32Bit() const
	{
		return m_ntHeaders.m_optionalHeader.m_is32bit;
	}

	Bool Executable::IsManaged() const
	{
		if (m_ntHeaders.m_optionalHeader.m_directories.size() <= DataDirectory::ComRuntimeDescriptor)
		{
			return False;
		}

		if (0 == m_ntHeaders.m_optionalHeader.m_directories[DataDirectory::ComRuntimeDescriptor].m_size)
		{
			return False;
		}

		if (0 == m_ntHeaders.m_optionalHeader.m_directories[DataDirectory::ComRuntimeDescriptor].m_virtualAddress)
		{
			return False;
		}

		return True;
	}

	const SectionHeader* Executable::RvaToSection(SizeT rva) const
	{
		auto section = std::find_if(
			begin(m_sections),
			end(m_sections),
			[rva](const SectionHeader& section)
			{
				return rva >= section.m_virtualAddress &&
					rva < section.m_virtualAddress + section.m_virtualSize;
			});
		
		if (m_sections.end() == section)
		{
			return nullptr;
		}

		return &(*section);
	}

	SizeT Executable::RvaToOffset(SizeT rva) const
	{
		auto section = RvaToSection(rva);
		if (section)
		{
			auto offset = rva - section->m_virtualAddress;
			if (section->m_sizeOfRawData < offset)
			{
				THROW_OUT_OF_RANGE_EXCEPTION();
			}

			return section->m_pointerToRawData + offset;
		}

		return rva;
	}

	SizeT Executable::GetImageBase() const
	{
		return (SizeT)m_ntHeaders.m_optionalHeader.GetImageBase();
	}

	const SectionArray& Executable::GetSections() const
	{
		return m_sections;
	}

	const DirectoryArray& Executable::GetDirectories() const
	{
		return m_ntHeaders.m_optionalHeader.m_directories;
	}

	const Export& Executable::GetExport() const
	{
		return m_export;
	}

	const Imports& Executable::GetImports() const
	{
		return m_imports;
	}

	const ExecutableChunks& Executable::GetCodeChunks() const
	{
		return m_codeChunks;
	}

	const ExecutableChunks& Executable::GetDataChunks() const
	{
		return m_dataChunks;
	}

	Void Executable::ParseExportDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ExportDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain exports
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			stream.SetPosition(offset);

			m_export.m_directory.Parse(stream);

			AddDataChunk(
				offset,
				stream.GetPosition(),
				directory.m_virtualAddress,
				"Export Directory");

			//
			// Let's parse export name pointer table
			//

			if (0 != m_export.m_directory.m_numberOfNames &&
				0 != m_export.m_directory.m_addressOfNames)
			{
				m_export.m_nameTable.resize(m_export.m_directory.m_numberOfNames);

				auto offset = RvaToOffset(m_export.m_directory.m_addressOfNames);
				stream.SetPosition(offset);

				stream.Read(
					(PByte)&m_export.m_nameTable[0],
					m_export.m_directory.m_numberOfNames * sizeof(m_export.m_nameTable[0]));
			
				AddDataChunk(
					offset,
					stream.GetPosition(),
					m_export.m_directory.m_addressOfNames,
					"Export Name Pointer Table");
			}

			//
			// Let's parse export ordinal table
			//

			if (0 != m_export.m_directory.m_numberOfNames &&
				0 != m_export.m_directory.m_addressOfNameOrdinals)
			{
				m_export.m_ordinalTable.resize(m_export.m_directory.m_numberOfNames);

				offset = RvaToOffset(m_export.m_directory.m_addressOfNameOrdinals);
				stream.SetPosition(offset);

				stream.Read(
					(PByte)&m_export.m_ordinalTable[0],
					m_export.m_directory.m_numberOfNames * sizeof(m_export.m_ordinalTable[0]));
			
				AddDataChunk(
					offset,
					stream.GetPosition(),
					m_export.m_directory.m_addressOfNameOrdinals,
					"Export Ordinal Table");
			}

			//
			// Let's parse export address table
			//

			if (0 != m_export.m_directory.m_numberOfFunctions &&
				0 != m_export.m_directory.m_addressOfFunctions)
			{
				m_export.m_addressTable.resize(m_export.m_directory.m_numberOfFunctions);

				auto offset = RvaToOffset(m_export.m_directory.m_addressOfFunctions);
				stream.SetPosition(offset);

				stream.Read(
					(PByte)&m_export.m_addressTable[0],
					m_export.m_directory.m_numberOfFunctions * sizeof(m_export.m_addressTable[0]));
			
				AddDataChunk(
					offset,
					stream.GetPosition(),
					m_export.m_directory.m_addressOfFunctions,
					"Export Address Table");
			}
			
			//
			// Let's parse module's export name
			//

			offset = RvaToOffset(m_export.m_directory.m_nameRva);
			stream.SetPosition(offset);

			m_export.m_name = ReadString(stream);
			m_export.m_name.ToLower();

			//AddDataChunk(
			//	offset,
			//	stream.GetPosition(),
			//	m_export.m_directory.m_nameRva,
			//	"Export Module Name");

			//
			// Lets parse exported symbols
			//
			
			UInt32 directoryBegin = directory.m_virtualAddress;
			UInt32 directoryEnd = directory.m_virtualAddress + directory.m_size;

			const SizeT addresstableSize = m_export.m_addressTable.size();
			for (SizeT i = 0; i < addresstableSize; i++)
			{
				const auto ordinal = (UInt16)i + m_export.m_directory.m_ordinalBase;
				
				const auto exportRva = m_export.m_addressTable[i];
				if (0 == exportRva)
				{
					auto symbol = std::make_shared<ExportSymbol>(exportRva, ordinal);
					m_export.m_ordinal2Symbol[ordinal] = std::move(symbol);
					// NOTE: We do not store null symbols in m_rva2Symbol by design!
					continue;
				}

				Bool isForwarder = False;
				if (exportRva > directoryBegin &&
					exportRva < directoryEnd)
				{
					isForwarder = True;
				}

				if (!isForwarder)
				{
					auto symbol = std::make_shared<ExportSymbol>(exportRva, ordinal);
					m_export.m_ordinal2Symbol[ordinal] = symbol;
					m_export.m_rva2Symbol[exportRva] = std::move(symbol);
					continue;
				}

				const auto offset = RvaToOffset(exportRva);
				stream.SetPosition(offset);

				auto forwarder = ReadString(stream);

				//AddDataChunk(
				//	offset,
				//	stream.GetPosition(),
				//	exportRva,
				//	"Export Forwarder Name");
					
				const auto dot = forwarder.Find(".");
				if ((SizeT)-1 == dot)
				{
					LOG_ERROR(L"Dot wasn't found in forwarder. NOTE: We should never reach this line.");
					continue;
				}

				auto targetModule = forwarder.SubString(0, dot);
				auto targetName = forwarder.SubString(dot + 1);

				targetModule.ToLower();
				targetModule.Append(".dll");

				Bool targetByOrdinal = False;
				UInt16 tagetOrdinal = 0;

				if (targetName.BeginsWith("#"))
				{
					targetByOrdinal = True;
					tagetOrdinal = (UInt16)atoi(targetName.SubString(1).GetBuffer());
				}

				if (!targetByOrdinal)
				{
					auto symbol = std::make_shared<ExportSymbol>(
							exportRva,
							ordinal,
							std::move(targetModule),
							std::move(targetName));
					m_export.m_ordinal2Symbol[ordinal] = symbol;
					m_export.m_rva2Symbol[exportRva] = std::move(symbol);
				}
				else
				{
					auto symbol = std::make_shared<ExportSymbol>(
							exportRva,
							ordinal,
							std::move(targetModule),
							tagetOrdinal);
					m_export.m_ordinal2Symbol[ordinal] = symbol;
					m_export.m_rva2Symbol[exportRva] = std::move(symbol);
				}
			}

			const SizeT nameTableSize = m_export.m_nameTable.size();
			for (SizeT i = 0; i < nameTableSize; i++)
			{
				const auto index = m_export.m_ordinalTable[i];
				const auto nameRva = m_export.m_nameTable[i];
				
				const auto nameOffset = RvaToOffset(nameRva);
				stream.SetPosition(nameOffset);

				auto name = ReadString(stream);

				//AddDataChunk(
				//	offset,
				//	stream.GetPosition(),
				//	nameRva,
				//	"Export Symbol Name");

				const auto ordinal = index + m_export.m_directory.m_ordinalBase;
				
				auto found = m_export.m_ordinal2Symbol.find(ordinal);
				if (std::end(m_export.m_ordinal2Symbol) == found)
				{
					LOG_ERROR(L"Export entry is Name Table and not in Address Table. NOTE: We should never reach this line.");
					continue;
				}

				m_export.m_name2Symbol[name] = found->second;

				found->second->m_name = std::move(name);
				found->second->m_byOrdinal = False;
			}
		}
		catch (OutOfRangeException& exception)
		{
			UNREFERENCED_PARAMETER(exception);
		}
	}

	Void Executable::ParseImportDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ImportDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain imports
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			stream.SetPosition(offset);

			SizeT position = 0;
			while (position < directory.m_size)
			{
				Import import;
				import.m_descriptor.Parse(stream);
			
				if (!import.m_descriptor.IsNull())
				{
					m_imports.push_back(import);
				}
				else
				{
					break;
				}
			
				position += sizeof(IMAGE_IMPORT_DESCRIPTOR);
			}

			AddDataChunk(
				offset,
				stream.GetPosition(),
				directory.m_virtualAddress,
				"Import Descriptors Table");

			//
			// Let's parse import modules' names
			//

			for (auto i = begin(m_imports); i != end(m_imports); i++)
			{
				Import& import = *i;
				auto offset = RvaToOffset(import.m_descriptor.m_nameRva);
				stream.SetPosition(offset);

				import.m_name = ReadString(stream);
				import.m_name.ToLower();

				AddDataChunk(
					offset,
					stream.GetPosition(),
					import.m_descriptor.m_nameRva,
					"Import Module Name");
			}

			//
			// Let's parse import address table
			//

			const SizeT size = Is32Bit() ? sizeof(UInt32) : sizeof(UInt64);

			for (auto i = begin(m_imports); i != end(m_imports); i++)
			{
				Import& import = *i;
				UInt64 thunk = 0;

				auto offset = RvaToOffset(import.m_descriptor.m_addressTableRva);
				stream.SetPosition(offset);

				while (true)
				{
					stream.Read((PByte)&thunk, size);
					if (0 == thunk)
					{
						break;
					}
					import.m_addressTable.push_back(thunk);
				}

				AddDataChunk(
					offset,
					stream.GetPosition(),
					import.m_descriptor.m_addressTableRva,
					"Import Address Table");
			}

			//
			// Let's parse import name table
			//

			for (auto i = begin(m_imports); i != end(m_imports); i++)
			{
				Import& import = *i;
				UInt64 thunk = 0;

				auto offset = RvaToOffset(import.m_descriptor.m_nameTableRva);
				stream.SetPosition(offset);

				while (true)
				{
					stream.Read((PByte)&thunk, size);
					if (0 == thunk)
					{
						break;
					}
					import.m_nameTable.push_back(thunk);
				}

				//AddDataChunk(
				//	offset,
				//	stream.GetPosition(),
				//	import.m_descriptor.m_nameTableRva,
				//	"Import Name Table");
			}

			//
			// Let's parse import symbols' names
			//

			const auto is32Bit = Is32Bit();

			for (auto i = begin(m_imports); i != end(m_imports); i++)
			{
				Import& import = *i;
				import.m_symbols.reserve(import.m_nameTable.size());

				for (auto j = begin(import.m_nameTable); j != end(import.m_nameTable); j++)
				{
					const auto thunk = *j;

					if ((is32Bit && thunk & 0x80000000) ||
						(!is32Bit && thunk & 0x8000000000000000))
					{
						// Import by ordinal
						const auto ordinal = (UInt16)thunk;

						import.m_symbols.push_back(ImportSymbol(ordinal));
					}
					else
					{
						// Import by name
						auto offset = RvaToOffset((SizeT)thunk);
						stream.SetPosition(offset);

						UInt16 hint;
						stream >> hint;

						import.m_symbols.push_back(ImportSymbol(ReadString(stream)));

						//AddDataChunk(
						//	offset,
						//	stream.GetPosition(),
						//	(UInt32)thunk,
						//	"Import Symbol Name");
					}
				}
			}
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseResourceDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ResourceDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain resources
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Resource Directory");
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseExceptionDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ExceptionDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain exception directory
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Exception Directory");
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseSecurityDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::SecurityDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];

			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain security directory
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Security Directory"); 
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseBaseRelocationTable(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::BaseRelocationTable;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain relocations
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Base Relocation Table");
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseDebugDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::DebugDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain debug info
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			stream.SetPosition(offset);

			auto count = directory.m_size / sizeof(IMAGE_DEBUG_DIRECTORY);
			while (count--)
			{
				DebugDirectory debugDirectory;
				debugDirectory.Parse(stream);

				AddDataChunk(
					debugDirectory.m_pointerToRawData,
					debugDirectory.m_pointerToRawData + debugDirectory.m_sizeOfData,
					debugDirectory.m_addressOfRawData,
					"Debug Data");
			}

			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Debug Directory");
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseArchitectureSpecificData(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ArchitectureSpecificData;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain architecture specific data
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Architecture Specific Data");
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseGlobalPointerTable(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::GlobalPointerTable;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];

			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain global pointer table
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Global Pointer Table"); 
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseThreadLocalStorageDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ThreadLocalStorageDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];

			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain TLS directory
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"TLS Directory"); 
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseLoadConfigurationDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::LoadConfigurationDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];

			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain load config
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			stream.SetPosition(offset);

			UInt32 size;
			stream >> size;

			AddDataChunk(
				offset,
				offset + size,
				directory.m_virtualAddress,
				"Load Configuration Directory"); 
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseBoundImportDirectory(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::BoundImportDirectory;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];

			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain bound imports
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Bound Import Directory");

			// TODO: We need to scan this 
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseImportAddressTable(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ImportAddressTable;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];
		
			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain import address table
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"Import Address Table");

			// TODO: We need to scan this
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseDelayImportDescriptors(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::DelayImportDescriptors;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];

			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain delay load imports
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			stream.SetPosition(offset);

			SizeT position = 0;
			while (position < directory.m_size)
			{
				DelayImportDescriptor desc;
				desc.Parse(stream);

				if (!desc.IsNull())
				{
					m_delayImportDescriptors.push_back(desc);
				}
				else
				{
					break;
				}

				position += 32; // Size of Delay Import Descriptor
			}

			AddDataChunk(
				offset,
				stream.GetPosition(),
				directory.m_virtualAddress,
				"Delay Import Descriptors Table");

			std::for_each(
				begin(m_delayImportDescriptors),
				end(m_delayImportDescriptors),
				[this, &stream](const DelayImportDescriptor& desc)
			{
				UInt64 thunk = 0;
				const SizeT size = Is32Bit() ? sizeof(UInt32) : sizeof(UInt64);

				auto firstThunk = RvaToOffset(desc.m_addressTableRva);
				stream.SetPosition(firstThunk);

				while (true)
				{
					stream.Read((PByte)&thunk, size);
					if (0 == thunk)
					{
						break;
					}
					m_delayImportAddressTable.push_back(thunk);
				}

				AddDataChunk(
					firstThunk,
					stream.GetPosition(),
					desc.m_addressTableRva,
					"Delay Import Address Table");

				auto originalFirstThunk = RvaToOffset(desc.m_nameTableRva);
				stream.SetPosition(originalFirstThunk);

				while (true)
				{
					stream.Read((PByte)&thunk, size);
					if (0 == thunk)
					{
						break;
					}
					m_delayImportNameTable.push_back(thunk);
				}

				AddDataChunk(
					originalFirstThunk,
					stream.GetPosition(),
					desc.m_nameTableRva,
					"Delay Import Name Table");
			});

			// TODO: We need to scan this 
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::ParseComRuntimeDescriptor(IStream& stream)
	{
		try
		{
			auto index = (SizeT)DataDirectory::ComRuntimeDescriptor;
			auto directory = m_ntHeaders.m_optionalHeader.m_directories[index];

			if (0 == directory.m_virtualAddress)
			{
				return; // Image doesn't contain com runtime descriptor
			}

			auto offset = RvaToOffset(directory.m_virtualAddress);
			AddDataChunk(
				offset,
				offset + directory.m_size,
				directory.m_virtualAddress,
				"COM Runtime descriptor"); 
		}
		catch (OutOfRangeException& exception)
		{
			exception;
		}
	}

	Void Executable::AddCodeChunk(SizeT begin, SizeT end, UInt32 rva, PCChar name)
	{
		if (end > begin)
		{
			m_codeChunks.AddChunk(ExecutableChunk(begin, end, name, rva));
		}
	}

	Void Executable::AddDataChunk(SizeT begin, SizeT end, UInt32 rva, PCChar name)
	{
		if (end > begin)
		{
			m_dataChunks.AddChunk(ExecutableChunk(begin, end, name, rva));
		}
	}

}