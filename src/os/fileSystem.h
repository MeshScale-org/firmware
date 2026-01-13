
#pragma once

#include <FileSystem.h>
#include <FileStream.h>
#include <Bytes.h>

#ifdef MCU_ESP32
#include <LittleFS.h>
#define fsImpl LittleFS
#elif MCU_NRF52
#include <InternalFileSystem.h>
#define fsImpl InternalFS
using namespace Adafruit_LittleFS_Namespace;
#endif

class fileSystem : public RNS::FileSystemImpl
{

public:
	fileSystem() {}

public:
	static void listDir(const char *dir);

public:
	virtual bool init();
	virtual bool file_exists(const char *file_path);
	virtual size_t read_file(const char *file_path, RNS::Bytes &data);
	virtual size_t write_file(const char *file_path, const RNS::Bytes &data);
	virtual RNS::FileStream open_file(const char *file_path, RNS::FileStream::MODE file_mode);
	virtual bool remove_file(const char *file_path);
	virtual bool rename_file(const char *from_file_path, const char *to_file_path);
	virtual bool directory_exists(const char *directory_path);
	virtual bool create_directory(const char *directory_path);
	virtual bool remove_directory(const char *directory_path);
	virtual std::list<std::string> list_directory(const char *directory_path);
	virtual size_t storage_size();
	virtual size_t storage_used();
	virtual size_t storage_available();

protected:
#if defined(MCU_ESP32) || defined(MCU_NRF52)
	class UniversalFileStream : public RNS::FileStreamImpl
	{

	private:
		std::unique_ptr<File> _file;
		bool _closed = false;

	public:
		UniversalFileStream(File *file) : RNS::FileStreamImpl(), _file(file) {}
		virtual ~UniversalFileStream()
		{
			if (!_closed)
				close();
		}

	public:
		inline virtual const char *name() { return _file->name(); }
		inline virtual size_t size() { return _file->size(); }
		inline virtual void close()
		{
			_closed = true;
			_file->close();
		}

		// Print overrides
		inline virtual size_t write(uint8_t byte) { return _file->write(byte); }
		inline virtual size_t write(const uint8_t *buffer, size_t size) { return _file->write(buffer, size); }

		// Stream overrides
		inline virtual int available() { return _file->available(); }
		inline virtual int read() { return _file->read(); }
		inline virtual int peek() { return _file->peek(); }
		inline virtual void flush() { _file->flush(); }
	};

#endif
};
