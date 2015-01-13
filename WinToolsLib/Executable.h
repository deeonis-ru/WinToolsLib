#pragma once
#include "Types.h"
#include "Buffer.h"
#include "IStream.h"
#include "IParsable.h"
#include "Exception.h"
#include "ExecutableChunk.h"
#include <list>
#include <map>
#include <memory>
#include <vector>

namespace WinToolsLib
{
	class DosHeader : public IParsable
	{
	public:
		DosHeader();

		Void Parse(IStream& stream) override;

	public:
		UInt32 m_ntHeadersOffset;
	};

	class FileHeader : public IParsable
	{
	public:
		FileHeader();

		Void Parse(IStream& stream) override;

	public:
		UInt16 m_machine;
		UInt16 m_numberOfSections;
		UInt32 m_timeDateStamp;
		UInt32 m_pointerToSymbolTable;
		UInt32 m_numberOfSymbols;
		UInt16 m_sizeOfOptionalHeader;
		UInt16 m_characteristics;
	};

	class DataDirectory : public IParsable
	{
	public:
		DataDirectory();

		Void Parse(IStream& stream) override;

		enum DirectoryEntry : SizeT
		{
			ExportDirectory,
			ImportDirectory,
			ResourceDirectory,
			ExceptionDirectory,
			SecurityDirectory,
			BaseRelocationTable,
			DebugDirectory,
			ArchitectureSpecificData,
			GlobalPointerTable,
			ThreadLocalStorageDirectory,
			LoadConfigurationDirectory,
			BoundImportDirectory,
			ImportAddressTable,
			DelayImportDescriptors,
			ComRuntimeDescriptor
		};

	public:
		UInt32 m_virtualAddress;
		UInt32 m_size;
	};

	typedef std::vector<DataDirectory> DirectoryArray;

	class OptionalHeader : public IParsable
	{
	public:
		OptionalHeader();

		Void Parse(IStream& stream) override;
		UInt64 GetImageBase() const;

	public:
		Bool   m_is32bit;
		UInt16 m_magic;
		UInt8  m_majorLinkerVersion;
		UInt8  m_minorLinkerVersion;
		UInt32 m_sizeOfCode;
		UInt32 m_sizeOfInitializedData;
		UInt32 m_sizeOfUninitializedData;
		UInt32 m_addressOfEntryPoint;
		UInt32 m_baseOfCode;
		union
		{
			struct
			{
				UInt32 m_baseOfData;
				UInt32 m_imageBase32;
			};
			UInt64 m_imageBase64;
		};
		UInt32 m_sectionAlignment;
		UInt32 m_fileAlignment;
		UInt16 m_majorOperatingSystemVersion;
		UInt16 m_minorOperatingSystemVersion;
		UInt16 m_majorImageVersion;
		UInt16 m_minorImageVersion;
		UInt16 m_majorSubsystemVersion;
		UInt16 m_minorSubsystemVersion;
		UInt32 m_win32VersionValue;
		UInt32 m_sizeOfImage;
		UInt32 m_sizeOfHeaders;
		UInt32 m_checkSum;
		UInt16 m_subsystem;
		UInt16 m_dllCharacteristics;
		union
		{
			struct
			{
				UInt32 m_sizeOfStackReserve32;
				UInt32 m_sizeOfStackCommit32;
				UInt32 m_sizeOfHeapReserve32;
				UInt32 m_sizeOfHeapCommit32;
			};
			struct
			{
				UInt64 m_sizeOfStackReserve64;
				UInt64 m_sizeOfStackCommit64;
				UInt64 m_sizeOfHeapReserve64;
				UInt64 m_sizeOfHeapCommit64;
			};
		};
		UInt32 m_loaderFlags;
		DirectoryArray m_directories;
	};

	class NtHeaders : public IParsable
	{
	public:
		NtHeaders();

		Void Parse(IStream& stream) override;

	public:
		FileHeader     m_fileHeader;
		OptionalHeader m_optionalHeader;
	};

	class SectionHeader : public IParsable
	{
	public:
		SectionHeader();

		Void Parse(IStream& stream) override;
		Bool IsExecutable() const;
		Bool IsDiscardable() const;
		Bool IsWriteable() const;

	public:
		StringA m_name;
		UInt32  m_virtualSize;
		UInt32  m_virtualAddress;
		UInt32  m_sizeOfRawData;
		UInt32  m_pointerToRawData;
		UInt32  m_pointerToRelocations;
		UInt32  m_pointerToLineNumbers;
		UInt16  m_numberOfRelocations;
		UInt16  m_numberOfLineNumbers;
		UInt32  m_characteristics;
	};

	typedef std::vector<SectionHeader> SectionArray;

	union Relocation
	{
		struct
		{
			UInt16 m_offset : 12;
			UInt16 m_type : 4;
		};
		UInt16 m_raw;
	};

	typedef std::vector<Relocation> RelocationArray;

	class RelocationBlock : public IParsable
	{
	public:
		RelocationBlock();

		Void Parse(IStream& stream) override;

	public:
		UInt32 m_virtualAddress;
		UInt32 m_size;
		RelocationArray m_relocations;
	};

	typedef std::vector<RelocationBlock> RelocationBlockArray;

	class ExportDirectory : public IParsable
	{
	public:
		ExportDirectory();

		Void Parse(IStream& stream) override;

	public:
		UInt32 m_characteristics;
		UInt32 m_timeDateStamp;
		UInt16 m_majorVersion;
		UInt16 m_minorVersion;
		UInt32 m_nameRva;
		UInt32 m_ordinalBase;
		UInt32 m_numberOfFunctions;
		UInt32 m_numberOfNames;
		UInt32 m_addressOfFunctions;
		UInt32 m_addressOfNames;
		UInt32 m_addressOfNameOrdinals;
	};

	class ExportSymbol
	{
	public:
		ExportSymbol();
		ExportSymbol(UInt32 rva, UInt16 ordinal);
		ExportSymbol(UInt32 rva, UInt16 ordinal, StringA name);
		ExportSymbol(UInt32 rva, UInt16 ordinal, StringA&& targetModule, UInt16 targetOrdinal);
		ExportSymbol(UInt32 rva, UInt16 ordinal, StringA&& targetModule, StringA&& targetName);
		ExportSymbol(UInt32 rva, UInt16 ordinal, StringA name, StringA&& targetModule, UInt16 targetOrdinal);
		ExportSymbol(UInt32 rva, UInt16 ordinal, StringA name, StringA&& targetModule, StringA&& targetName);
		ExportSymbol(const ExportSymbol& other);
		ExportSymbol(ExportSymbol&& other);

		Bool IsNull() const;
		StringA GetName() const;
		
	public:
		UInt32  m_rva;
		Bool    m_byOrdinal;
		UInt16  m_ordinal;
		StringA m_name;
		Bool    m_isForwarded;
		StringA m_targetModule;
		Bool    m_targetByOrdinal;
		UInt16  m_targetOrdinal;
		StringA m_targetName;
	};

	typedef std::shared_ptr<ExportSymbol> ExportSymbolSharedPtr;

	typedef std::map<UInt32,  ExportSymbolSharedPtr> ExportRva2SymbolMap;
	typedef std::map<StringA, ExportSymbolSharedPtr> ExportName2SymbolMap;
	typedef std::map<UInt16,  ExportSymbolSharedPtr> ExportOrdinal2SymbolMap;

	typedef std::vector<UInt16> ExportOrdinals;
	typedef std::vector<UInt32> ExportNames;
	typedef std::vector<UInt32> Exports;

	class Export
	{
	public:
		Export();
		Export(const Export& other);
		Export(Export&& other);

		const ExportSymbol* GetSymbol(UInt16 ordinal) const;
		const ExportSymbol* GetSymbol(const StringA& name) const;
		const ExportSymbol* GetNearestSymbol(UInt32 rva, UInt32& offset) const;
		StringA             GetNearestSymbolName(UInt32 rva) const;
		ExportRva2SymbolMap GetSymbolMap(UInt32 rva, UInt32 size) const;

		StringA                 m_name;
		ExportDirectory         m_directory;
		ExportOrdinals          m_ordinalTable;
		ExportNames             m_nameTable;
		Exports                 m_addressTable;
		ExportRva2SymbolMap     m_rva2Symbol;
		ExportName2SymbolMap    m_name2Symbol;
		ExportOrdinal2SymbolMap m_ordinal2Symbol;
	};

	class ImportDescriptor : public IParsable
	{
	public:
		ImportDescriptor();

		Void Parse(IStream& stream) override;

		Bool IsNull() const;

	public:
		UInt32 m_nameTableRva;
		UInt32 m_timeDateStamp;
		UInt32 m_forwarderChain;
		UInt32 m_nameRva;
		UInt32 m_addressTableRva;
	};

	typedef std::vector<UInt64> Thunks;

	class ImportSymbol
	{
	public:
		ImportSymbol();
		ImportSymbol(UInt16 ordinal);
		ImportSymbol(StringA&& name);
		ImportSymbol(const ImportSymbol& other);
		ImportSymbol(ImportSymbol&& other);

		StringA GetName() const;
		
	public:
		Bool    m_byOrdinal;
		UInt16  m_ordinal;
		StringA m_name;
	};

	typedef std::vector<ImportSymbol> ImportSymbols;

	class Import
	{
	public:
		Import();
		Import(const Import& other);
		Import(Import&& other);

		ImportDescriptor m_descriptor;
		Thunks           m_nameTable;
		Thunks           m_addressTable;
		StringA          m_name;
		ImportSymbols    m_symbols;
	};

	typedef std::vector<Import> Imports;

	class DelayImportDescriptor : public IParsable
	{
	public:
		DelayImportDescriptor();

		Void Parse(IStream& stream) override;

		Bool IsNull() const;

	public:
		UInt32 m_attributes;
		UInt32 m_dllNameRva;
		UInt32 m_hModuleRva;
		UInt32 m_addressTableRva;
		UInt32 m_nameTableRva;
		UInt32 m_boundIatRva;
		UInt32 m_unloadIatRva;
		UInt32 m_timeDateStamp;
	};

	typedef std::vector<DelayImportDescriptor> DelayImportDescriptors;

	class DebugDirectory : public IParsable
	{
	public:
		DebugDirectory();

		Void Parse(IStream& stream) override;

	public:
		UInt32 m_characteristics;
		UInt32 m_timeDateStamp;
		UInt16 m_majorVersion;
		UInt16 m_minorVersion;
		UInt32 m_type;
		UInt32 m_sizeOfData;
		UInt32 m_addressOfRawData;
		UInt32 m_pointerToRawData;
	};

	class Executable : public IParsable
	{
	public:
		Executable(Bool isKernel);
		
		Void Parse(IStream& stream) override;
		Void Relocate(IStream& stream, PByte newBase);

		Void AddDataChunk(UInt32 rva, SizeT size, PCChar name);
		Void AnalyzeChunks();

		StringA ReadString(IStream& stream) const;
		StringA ReadString(IStream& stream, SizeT rva) const;
		StringA GetExportName(IStream& stream, UInt16 ordinal) const;
		UInt32 GetExportAddress(IStream& stream, StringA name) const;

		Bool Is32Bit() const;
		Bool IsManaged() const;

		const SectionHeader* RvaToSection(SizeT rva) const;
		SizeT RvaToOffset(SizeT rva) const;
		SizeT GetImageBase() const;

		const SectionArray& GetSections() const;
		const DirectoryArray& GetDirectories() const;

		const Export& GetExport() const;
		const Imports& GetImports() const;

		const ExecutableChunks& GetCodeChunks() const;
		const ExecutableChunks& GetDataChunks() const;

	protected:
		Void ParseExportDirectory(IStream& stream);
		Void ParseImportDirectory(IStream& stream);
		Void ParseResourceDirectory(IStream& stream);
		Void ParseExceptionDirectory(IStream& stream);
		Void ParseSecurityDirectory(IStream& stream);
		Void ParseBaseRelocationTable(IStream& stream);
		Void ParseDebugDirectory(IStream& stream);
		Void ParseArchitectureSpecificData(IStream& stream);
		Void ParseGlobalPointerTable(IStream& stream);
		Void ParseThreadLocalStorageDirectory(IStream& stream);
		Void ParseLoadConfigurationDirectory(IStream& stream);
		Void ParseBoundImportDirectory(IStream& stream);
		Void ParseImportAddressTable(IStream& stream);
		Void ParseDelayImportDescriptors(IStream& stream);
		Void ParseComRuntimeDescriptor(IStream& stream);

		Void AddCodeChunk(SizeT begin, SizeT end, UInt32 rva, PCChar name);
		Void AddDataChunk(SizeT begin, SizeT end, UInt32 rva, PCChar name);
		
	private:
		DosHeader              m_dosHeader;
		NtHeaders              m_ntHeaders;
		SectionArray           m_sections;
		Export                 m_export;
		Imports                m_imports;
		DelayImportDescriptors m_delayImportDescriptors;
		Thunks                 m_delayImportAddressTable;
		Thunks                 m_delayImportNameTable;
		ExecutableChunks       m_codeChunks;
		ExecutableChunks       m_dataChunks;

		Bool                   m_isKernel;
	};

	typedef std::tr1::shared_ptr<Executable> ExecutableSharedPtr;
}
