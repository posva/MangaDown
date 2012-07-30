#ifndef _INIFILE_HPP_
#define _INIFILE_HPP_

#include <map>
#include <fstream>

/*******************************************************************************
 * SimpleIniParser
 * A really simple .ini file parser
 *
 * Does not support comments, formating is minimalistic
 *
 * Started on 08 June 2012
 * Senryoku <maretverdant@gmail.com>
 * https://github.com/Senryoku/SimpleIniParser
 ******************************************************************************/

namespace Ini {
	class Pair : public std::pair<std::string, std::string>
	{
		public:
			Pair(std::pair<std::string, std::string> P) : std::pair<std::string, std::string>(P) {}

			const std::string& getKey() { return first; }
			const std::string& getValue() { return second; }
	};

	class Section : public std::map<std::string, std::string>
	{
		private:
			std::string myName;
		public:
			Section(std::string Name) : myName(Name) {}

			const std::string& getName() { return myName; }
			const std::string& getValue(const std::string& Key) { return (*this)[Key]; }
	};
}

using namespace Ini;

class IniFile : public std::map<std::string, Section*>
{
	public:
		/** @brief Default constructor */
		IniFile();
		/** @brief Constructor with file
		 *
		 * Loads the given file
		 * @param Path Path to an existing .ini file
		**/
		IniFile(const std::string& Path);
		/** @brief Default destructor */
		~IniFile();

		/** @brief Return current file */
		std::string getPath() { return myPath; }
		/** @brief Sets path used by save() and load() */
		void setPath(const std::string& Path) { myPath = Path; }

		/** @brief Re-Loads current file */
		bool load();
		/** @brief Loads given file, update internal Path
		 *
		 * @param Path File to load
		**/
		bool load(const std::string& Path);
		/** @brief Saves current file */
		bool save();
		/** @brief Saves content into given file
		 *
		 * @param Path Ini File
		**/
		bool save(const std::string& Path);

		/** @brief Frees current data **/
		void free();

		/** @brief Add a section
		 *
		 * @param Name New section's name
		**/
		void addSection(const std::string& Name);

		/** @brief Add/Modify a value
		 *
		 * Creates Section if doesn't exist
		 * If [Section]Key is already specified, replace associated value.
		 * @param Name Section's name
		 * @param Key Key
		 * @param Value Value
		**/
		void addValue(const std::string& Name, const std::string& Key, const std::string& Value = "");
		void addKey(const std::string& Name, const std::string& Key, const std::string& Value = "") { addValue(Name, Key, Value); }

		/** @brief Returns section "Name"
		 *
		 * @param Name Section's name
		 * @return Section
		**/
		Section* getSection(const std::string&Name);

		/** @brief Tests if a section exists
		 *
		 * @param Name Section's name
		 * @return true if section exists
		**/
		bool isSection(const std::string& Name);

		/** @brief Tests if a key (in a given section) exists
		 *
		 * @param Name Section's name
		 * @param Key Key
		 * @return true if section/key exists
		**/
		bool isKey(const std::string& Name, const std::string& Key);

		/** @brief Search for a value
		 *
		 * @param Name Section's name
		 * @param Key Key
		 * @return Value
		**/
		std::string getValue(const std::string& Name, const std::string& Key);

		/** @brief Returns sections count
		 *
		 * @return Sections count
		**/
		unsigned int getSectionCount() { return (*this).size(); }

		/** @brief Used to iterate on Sections
		 *
		 * Example :
		 * @code
		 * for(IniFile::const_iterator it = Config.getIterator();
		 * it != Config.getEnd(); it++)
		 * 	std::cout << "[" << it->first << "]" << std::endl;
		 * @see getEnd()
		**/
		IniFile::const_iterator getIterator() { return (*this).begin(); }

		/** @brief Used to iterate on Sections (End test)
		 *
		**/
		IniFile::const_iterator getEnd() { return (*this).end(); }

	private:
		std::string myPath; ///< Current File
};

#endif // _INIFILE_HPP_
