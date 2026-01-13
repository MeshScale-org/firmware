

#include "fileSystem.h"

#include <Utilities/OS.h>
#include <Log.h>

void fileSystem::listDir(const char *dir)
{
	Serial.print("DIR: ");
	Serial.println(dir);
	File root = fsImpl.open(dir);
	if (!root)
	{
		Serial.println("Failed to opend directory");
		return;
	}
	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			Serial.print("  DIR: ");
		}
		else
		{
			Serial.print("  FILE: ");
		}
		Serial.println(file.name());
		file.close();
		file = root.openNextFile();
	}
	root.close();
}

/*virtual*/ bool fileSystem::init()
{
	TRACE("fileSystem initializing...");

#ifdef MCU_ESP32
	// Setup FileSystem
	INFO("fsImpl (ESP32) mounting FileSystem");
	if (!fsImpl.begin(true, ""))
	{
		ERROR("fsImpl (ESP32) FileSystem mount failed");
		return false;
	}
	INFO("fsImpl (ESP32) FileSystem is ready");
	uint32_t flashSize = spi_flash_get_chip_size();
	INFOF("Total flash size: %d KB", flashSize / 1024);
	uint32_t ram_size = ESP.getHeapSize();
	INFOF("Total RAM size: %d KB", ram_size / 1024);
#elif MCU_NRF52
	// Initialize Internal File System
	INFO("fsImpl (NRF52) mounting FileSystem");
	if (!fsImpl.begin())
	{
		ERROR("fsImpl (NRF) FileSystem mount failed");
		return false;
	}
	INFO("fsImpl (NRF52) FileSystem is ready");
	uint32_t flashSize = NRF_FICR->CODEPAGESIZE * NRF_FICR->CODESIZE;
	INFOF("Total flash size: %d KB", flashSize / 1024);
	uint32_t ram_size = NRF_FICR->INFO.RAM; // already in KB
	INFOF("Total RAM size: %d KB", ram_size);
#endif

	uint32_t size = storage_size() / (1024);
	INFOF("Filesystem size: %d KB", size);
	uint32_t used = storage_used() / (1024);
	INFOF("Filesystem used: %d KB", used);
	// ensure FileSystem is writable and format if not
	RNS::Bytes test("test");
	if (write_file("/test", test) < 4)
	{
		INFO("fsImpl FileSystem is being formatted, please wait...");
		fsImpl.format();
	}
	else
	{
		remove_file("/test");
	}

	return true;
}

/*virtual*/ bool fileSystem::file_exists(const char *file_path)
{
#ifdef MCU_ESP32
	File file = fsImpl.open(file_path, FILE_READ);
	if (file)
	{
#elif MCU_NRF52
	File file(fsImpl);
	if (file.open(file_path, FILE_O_READ))
	{
#else
	if (false)
	{
#endif

		// TRACE("file_exists: file exists, closing file");

		file.close();

		return true;
	}
	else
	{
		ERROR("file_exists: failed to open file " + std::string(file_path));
		return false;
	}
}

/*virtual*/ size_t fileSystem::read_file(const char *file_path, RNS::Bytes &data)
{
	size_t read = 0;

#ifdef MCU_ESP32
	File file = fsImpl.open(file_path, FILE_READ);
	if (file)
	{
		size_t size = file.size();
		read = file.readBytes((char *)data.writable(size), size);
#elif MCU_NRF52
	// File file(fsImpl);
	// if (file.open(file_path, FILE_O_READ)) {
	File file = fsImpl.open(file_path, FILE_O_READ);
	if (file)
	{
		size_t size = file.size();
		read = file.readBytes((char *)data.writable(size), size);
#else
	if (false)
	{
		size_t size = 0;
#endif

		TRACE("read_file: read " + std::to_string(read) + " bytes from file " + std::string(file_path));
		if (read != size)
		{
			ERROR("read_file: failed to read file " + std::string(file_path));
			data.clear();
		}
		// TRACE("read_file: closing input file");

		file.close();
	}
	else
	{
		ERROR("read_file: failed to open input file " + std::string(file_path));
	}
	return read;
}

/*virtual*/ size_t fileSystem::write_file(const char *file_path, const RNS::Bytes &data)
{
	// CBA TODO Replace remove with working truncation
	remove_file(file_path);
	size_t wrote = 0;

#ifdef MCU_ESP32
	File file = fsImpl.open(file_path, FILE_WRITE);
	if (file)
	{
		wrote = file.write(data.data(), data.size());
#elif MCU_NRF52
	File file(fsImpl);
	if (file.open(file_path, FILE_O_WRITE))
	{
		wrote = file.write(data.data(), data.size());
#else
	if (false)
	{
#endif

		TRACE("write_file: wrote " + std::to_string(wrote) + " bytes to file " + std::string(file_path));
		if (wrote < data.size())
		{
			WARNING("write_file: not all data was written to file " + std::string(file_path));
		}
		// TRACE("write_file: closing output file");

		file.close();
	}
	else
	{
		ERROR("write_file: failed to open output file " + std::string(file_path));
	}
	return wrote;
}

/*virtual*/ RNS::FileStream fileSystem::open_file(const char *file_path, RNS::FileStream::MODE file_mode)
{
	TRACEF("open_file: opening file %s", file_path);

#ifdef MCU_ESP32
	const char *mode;
	if (file_mode == RNS::FileStream::MODE_READ)
	{
		mode = FILE_READ;
	}
	else if (file_mode == RNS::FileStream::MODE_WRITE)
	{
		mode = FILE_WRITE;
	}
	else if (file_mode == RNS::FileStream::MODE_APPEND)
	{
		mode = FILE_APPEND;
	}
	else
	{
		ERRORF("open_file: unsupported mode %d", file_mode);
		return {RNS::Type::NONE};
	}
	TRACEF("open_file: opening file %s in mode %s", file_path, mode);
	//// Using copy constructor to create a File* instead of local
	// File file = fsImpl.open(file_path, mode);
	// if (!file) {
	File *file = new File(fsImpl.open(file_path, mode));
	if (file == nullptr || !(*file))
	{
		ERRORF("open_file: failed to open output file %s", file_path);
		return {RNS::Type::NONE};
	}
	TRACEF("open_file: successfully opened file %s", file_path);
	return RNS::FileStream(new UniversalFileStream(file));
#elif MCU_NRF52
	// File file = File(fsImpl);
	File *file = new File(fsImpl);
	int mode;
	if (file_mode == RNS::FileStream::MODE_READ)
	{
		mode = FILE_O_READ;
	}
	else if (file_mode == RNS::FileStream::MODE_WRITE)
	{
		mode = FILE_O_WRITE;
		// CBA TODO Replace remove with working truncation
		if (fsImpl.exists(file_path))
		{
			fsImpl.remove(file_path);
		}
	}
	else if (file_mode == RNS::FileStream::MODE_APPEND)
	{
		// CBA This is the default write mode for MCU_NRF52 littlefs
		mode = FILE_O_WRITE;
	}
	else
	{
		ERRORF("open_file: unsupported mode %d", file_mode);
		return {RNS::Type::NONE};
	}
	if (!file->open(file_path, mode))
	{
		ERRORF("open_file: failed to open output file %s", file_path);
		return {RNS::Type::NONE};
	}

	// Seek to beginning to overwrite (this is failing on MCU_NRF52)
	// if (file_mode == RNS::FileStream::MODE_WRITE) {
	//	file->seek(0);
	//	file->truncate(0);
	//}
	TRACEF("open_file: successfully opened file %s", file_path);
	return RNS::FileStream(new UniversalFileStream(file));
#else
#warning ("unsuppoprted");
	return RNS::FileStream(RNS::Type::NONE);
#endif
}

/*virtual*/ bool fileSystem::remove_file(const char *file_path)
{
#if defined(MCU_ESP32) || defined(MCU_NRF52)
	return fsImpl.remove(file_path);
#else
	return false;
#endif
}

/*virtual*/ bool fileSystem::rename_file(const char *from_file_path, const char *to_file_path)
{
#if defined(MCU_ESP32) || defined(MCU_NRF52)
	return fsImpl.rename(from_file_path, to_file_path);
#else
	return false;
#endif
}

/*virtua*/ bool fileSystem::directory_exists(const char *directory_path)
{
	TRACE("directory_exists: checking for existence of directory " + std::string(directory_path));

#ifdef MCU_ESP32
	File file = fsImpl.open(directory_path, FILE_READ);
	if (file)
	{
		bool is_directory = file.isDirectory();
		file.close();
		return is_directory;
	}
#elif MCU_NRF52
	File file(fsImpl);
	if (file.open(directory_path, FILE_O_READ))
	{
		bool is_directory = file.isDirectory();
		file.close();
		return is_directory;
	}
#endif
	return false;
}

/*virtual*/ bool fileSystem::create_directory(const char *directory_path)
{
#if defined(MCU_ESP32) || defined(MCU_NRF52)
	if (!fsImpl.mkdir(directory_path))
	{
		ERROR("create_directory: failed to create directorty " + std::string(directory_path));
		return false;
	}
	return true;
#else
	return false;
#endif
}

/*virtua*/ bool fileSystem::remove_directory(const char *directory_path)
{
	TRACE("remove_directory: removing directory " + std::string(directory_path));

#ifdef MCU_ESP32
	if (!fsImpl.rmdir(directory_path))
	{
		ERROR("remove_directory: failed to remove directorty " + std::string(directory_path));
		return false;
	}
	return true;
#elif MCU_NRF52
	if (!fsImpl.rmdir_r(directory_path))
	{
		ERROR("remove_directory: failed to remove directory " + std::string(directory_path));
		return false;
	}
	return true;
#else
	return false;
#endif
}

/*virtua*/ std::list<std::string> fileSystem::list_directory(const char *directory_path)
{
	TRACE("list_directory: listing directory " + std::string(directory_path));
	std::list<std::string> files;

#if defined(MCU_ESP32) || defined(MCU_NRF52)
	File root = fsImpl.open(directory_path);
#endif
	if (!root)
	{
		ERROR("list_directory: failed to open directory " + std::string(directory_path));
		return files;
	}
	File file = root.openNextFile();
	while (file)
	{
		if (!file.isDirectory())
		{
			char *name = (char *)file.name();
			files.push_back(name);
		}
		// CBA Following close required to avoid leaking memory
		file.close();
		file = root.openNextFile();
	}
	TRACE("list_directory: returning directory listing");
	root.close();
	return files;
}

#ifdef MCU_NRF52

static int _countLfsBlock(void *p, lfs_block_t block)
{
	lfs_size_t *size = (lfs_size_t *)p;
	*size += 1;
	return 0;
}

static lfs_ssize_t getUsedBlockCount()
{
	lfs_size_t size = 0;
	lfs_traverse(fsImpl._getFS(), _countLfsBlock, &size);
	return size;
}

static int totalBytes()
{
	const lfs_config *config = fsImpl._getFS()->cfg;
	return config->block_size * config->block_count;
}

static int usedBytes()
{
	const lfs_config *config = fsImpl._getFS()->cfg;
	const int usedBlockCount = getUsedBlockCount();
	return config->block_size * usedBlockCount;
}
#endif

/*virtual*/ size_t fileSystem::storage_size()
{

#ifdef MCU_NRF52
	return totalBytes();
#elif MCU_ESP32
	return fsImpl.totalBytes();
#endif
}

/*virtual*/ size_t fileSystem::storage_used()
{
#ifdef MCU_NRF52
	return usedBytes();
#elif MCU_ESP32
	return fsImpl.usedBytes();
#endif
}

/*virtual*/ size_t fileSystem::storage_available()
{

#ifdef MCU_NRF52
	return totalBytes() - usedBytes();
#elif MCU_ESP32
	return (fsImpl.totalBytes() - fsImpl.usedBytes());
#endif
}
