// © Copyright 2020 Ollivier Roberge
#include <ostream>
#include <istream>
#include <version>

namespace Aulit::io {
#ifdef __cpp_lib_syncbuf
	#include <syncstream>
	template<typename... Args> inline void synced_writeln(std::ostream& stream, const Args& ...args) { (std::osyncstream(stream) << ... << args) << '\n'; }
	template<typename... Args> inline void synced_write(std::ostream& stream, const Args& ...args) { (std::osyncstream(stream) << ... << args); }
#endif // __cpp_lib_syncbuf
	template<typename... Args> inline void writeln(std::ostream& stream, const Args& ...args) { (stream << ... << args) << '\n'; }
	template<typename... Args> inline void write(std::ostream& stream, const Args& ...args) { (stream << ... << args); }
	template<typename... Args> inline void read(std::istream& stream, const Args& ...args) { (stream >> ... >> args); }
}