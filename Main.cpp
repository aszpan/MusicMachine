// Programming assignment MusicMachine.
// Macomb Community College

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "json.hpp"


using namespace std;
using json = nlohmann::json;

////
const std::string DATA_FILE_PATH = "TestData\\";

namespace MusicMachine
{
	class Music;	// Forward declaration
}

std::ostream& operator<<(std::ostream& out, const MusicMachine::Music& ex_machina);

namespace MusicMachine
{
	enum class MUSIC_TYPE
	{
		UNKNOWN,
		CHAMBER,
		ORCHESTRAL,
		ROCK,
		ELECTRONIC,
		POP,
		JAZZ,
		BLUES
	};

	class Music
	{
	protected:

		string ArtistName;
		string Location;
		string Origin;

		MUSIC_TYPE MusicType{};

		enum class TEMPO
		{
			UNKNOWN,
			SLOW,
			FAST
		};
		TEMPO Tempo{};

	public:

		MUSIC_TYPE GetMusicType() const
		{
			return MusicType;
		}

		bool IsTempoFast() const
		{
			switch (Tempo) {
			case TEMPO::FAST:
				return true;
				break;
			case TEMPO::SLOW:
			case TEMPO::UNKNOWN:
				return false;
				break;
			}
			return false;

		}

		//constructor
		//does not need explicit, can remove
		Music(json& j, MUSIC_TYPE type, TEMPO tempo)
		{
			Tempo = tempo;
			ArtistName = j["Artist"];
			Origin = j["Origin"];
			Location = j["Location"];
			MusicType = type;
		}

		//Destructor
		virtual ~Music() {}

		//overload operator == using boolean
		// bool is used because == is a logical operator that is asking does this match. only 2 answers y/n t/f 1/0
		//should use an additional attribute like id to compare IDs rather than names but this will work
		virtual bool operator==(const Music& m) const {
			return this->MusicType == m.MusicType;
		}

		// pure virtual functions force all derived classes to make their own implementation of the f(x)
		// using a pure virtual class causes the class to become "abstract". Abstract classes have
		//at least one pure virtual function
		virtual void Play() const = 0;

		// change all PrettyPrintClassAttributes()'s to virtual voids
		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const
		{
			out << ArtistName << "," << Origin << "," << Location;
			out << endl;
		}


	};

	class Classical : public Music
	{
	protected:
		string Key;
		string PieceName;

	public:

		//Constructor
		Classical(json& j, MUSIC_TYPE type, TEMPO tempo) : Music(j, type, tempo)
		{
			PieceName = j["Name"];
			Key = j["Key"];
		}

		//Destructor
		virtual ~Classical() {}

		// overload operator == using boolean
		// bool is used because == is a logical operator that is asking does this match.
		// only 2 answers y/n t/f 1/0
		// should use an additional attribute like id to compare IDs rather than names but
		// this will work
		virtual bool operator==(const Classical& c) const {
			return this->MusicType == c.MusicType;
		}

		string GetKey() {
			// need to write something here so key A-G will increment with the ++ operator
			// and decrement with -- operator.
			// this function is a string. the ++/-- operators increment ints.
			return Key;
		}

		// unsure what to do with this one
		// https://en.cppreference.com/w/cpp/language/operators
		//////////////////////////////////////////
		Classical& operator++(int u) {
			// do stuff
			// GetKey() = *this; // copy old value
			// operator++(int u);  // prefix increment
			// return GetKey();    // return old value
			if (this->Key == "G") {
				this->Key = "A";
			}
			else
			{
				++(this->Key[0]);
			}
			return *this;
		}

		virtual void Play() const
		{
			cout << "Now playing CLASSICAL Music..." << endl;
		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const
		{
			out << ArtistName << "," << Origin << "," << Location << "," << PieceName << "," << Key << ",";
		}

	};

	class Chamber : public Classical {
		struct Instruments
		{
			string One;
			string Two;
			string Three;
			string Four;

		};

		Instruments Instruments;
	public:

		//Constructor
		Chamber(json& j) : Classical(j, MUSIC_TYPE::CHAMBER, TEMPO::SLOW)
		{
			Instruments.One = j["Instruments"]["1"];
			Instruments.Two = j["Instruments"]["2"];
			Instruments.Three = j["Instruments"]["3"];
			Instruments.Four = j["Instruments"]["4"];

		}

		//Destructor
		virtual ~Chamber() {}

		virtual bool operator==(const Chamber& c) const {
			return this->MusicType == c.MusicType;
		}

		virtual void Play() const
		{
			cout << "Now playing Chamber Music..." << endl;

		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const
		{
			Classical::PrettyPrintClassAttributes(out);

			out << Instruments.One << ","
				<< Instruments.Two << ","
				<< Instruments.Three << ","
				<< Instruments.Four
				<< endl;
		}

	};

	class Orchestral : public Classical
	{

	private:
		vector <string> Instruments;
		string Key;


	public:

		//Constructor
		Orchestral(json& j) : Classical(j, MUSIC_TYPE::ORCHESTRAL, TEMPO::SLOW)
		{
			Key = j["Key"];

			//should check if is_array
			if (j["Instruments"].is_array()) {
				for (int i = 0; i < j["Instruments"].size(); i++) {
					// adding elements to vector
					Instruments.push_back(j["Instruments"][i]);

				}

			}

		}

		//Destructor
		virtual ~Orchestral() {}

		virtual bool operator==(const Orchestral& o) const {
			return this->MusicType == o.MusicType;
		}

		virtual void Play() const
		{
			cout << "Now playing Orchestral Music..." << endl;

		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const {
			Classical::PrettyPrintClassAttributes(out);

			string strInstruments;

			// adding elements from vector to single string (strInstruments) in order to print
			//ask Sammi why she used this particular style
			for (auto& i : Instruments) {

				if (i == Instruments.at(0))
					strInstruments += i;
				else
					strInstruments = strInstruments + "," + i;
			}

			cout << strInstruments << endl;
		}


	};

	class NonClassical : public Music
	{

		string SongName;

	public:

		//Constructor
		explicit NonClassical(json& j, MUSIC_TYPE type, TEMPO tempo) : Music(j, type, tempo)
		{
			SongName = j["Name"];

		}

		//Destructor
		virtual ~NonClassical() {}

		// overload operator == using boolean
			// bool is used because == is a logical operator that is asking does this match. only 2 answers y/n t/f 1/0
			//should use an additional attribute like id to compare IDs rather than names but this will work
		virtual bool operator==(const NonClassical& nc) const {
			return this->MusicType == nc.MusicType;
		}

		virtual void Play() const
		{
			cout << "Now playing NONCLASSICAL Music..." << endl;

		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const
		{
			out << ArtistName << "," << Origin << "," << Location << "," << SongName << ",";
		}
	};

	class Pop : public NonClassical
	{

		string DateOfRanking;
		int Top40Ranking;


	public:

		//Constructor
		Pop(json& j) : NonClassical(j, MUSIC_TYPE::POP, TEMPO::FAST)
		{

			Top40Ranking = j["Rank"];
			DateOfRanking = j["DateOfRanking"];

		}

		//Destructor
		virtual ~Pop() {}

		virtual bool operator==(const Pop& p) const {
			return this->MusicType == p.MusicType;
		}
		virtual bool operator<(const Pop& p) const {
			return true;
		}
		virtual bool operator>(const Pop& p) const {
			return true;
		}

		virtual void Play() const
		{
			cout << "Now playing Pop Music......" << endl;

		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const
		{
			NonClassical::PrettyPrintClassAttributes(out);
			cout << DateOfRanking << "," << to_string(Top40Ranking);
			cout << endl;
		}

	};

	class Jazz : public NonClassical
	{

		enum class IMPROVISATION
		{
			NONE,
			SOME,
			ALOT
		};

		string ImprovisationToString() const
		{

			string str;
			if (Improvisation == IMPROVISATION::NONE)
				str = "None";
			else if (Improvisation == IMPROVISATION::SOME)
				str = "Some";
			else if (Improvisation == IMPROVISATION::ALOT)
				str = "Alot";
			else
				cerr << "Error! Unknown type" << endl;

			return str;

		}

		IMPROVISATION Improvisation{};

	public:

		//Constructor
		Jazz(json& j) : NonClassical(j, MUSIC_TYPE::JAZZ, TEMPO::FAST)
		{

			if (j["Improvisation"] == "Some")
				Improvisation = IMPROVISATION::SOME;
			else if (j["Improvisation"] == "None")
				Improvisation = IMPROVISATION::NONE;
			else if (j["Improvisation"] == "Alot")
				Improvisation = IMPROVISATION::ALOT;
			else
				cerr << "Unknown Improvisation Type" << endl;



		}

		//Destructor
		virtual ~Jazz() {}

		virtual bool operator==(const Jazz& j) const {
			return this->MusicType == j.MusicType;
		}

		virtual void Play() const
		{
			cout << "Now playing JAZZ Music......" << endl;

		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const
		{
			NonClassical::PrettyPrintClassAttributes(out);
			string str = ImprovisationToString();
			cout << str;
			cout << endl;

		}

	};

	class Rock : public NonClassical {

		enum class VOLUME {
			NA,
			QUIET,
			LOUD,
			VERYLOUD
		};

		int Expletives;
		bool GuitarsOnly;



		string VolumeToString() const
		{
			string str;
			if (Volume == VOLUME::NA)
				str = "NA";
			else if (Volume == VOLUME::QUIET)
				str = "Quiet";
			else if (Volume == VOLUME::LOUD)
				str = "Loud";
			else if (Volume == VOLUME::VERYLOUD)
				str = "Very Loud";
			else
				cerr << "Error! Unknown type" << endl;

			return str;

		}

		VOLUME Volume{};

	public:

		//Constructor
		Rock(json& j) : NonClassical(j, MUSIC_TYPE::ROCK, TEMPO::FAST)
		{

			GuitarsOnly = j["Guitars Only"];
			Expletives = j["Expletives"];

			if (j["Recommended Volume"] == "NA")
				Volume = VOLUME::NA;
			else if (j["Recommended Volume"] == "Quiet")
				Volume = VOLUME::QUIET;
			else if (j["Recommended Volume"] == "Loud")
				Volume = VOLUME::LOUD;
			else if (j["Recommended Volume"] == "Very Loud")
				Volume = VOLUME::VERYLOUD;
			else
				cerr << "Error! Unknown type" << endl;

		}

		bool IsLoud()
		{
			return Volume == VOLUME::LOUD;
		}
		bool IsQuiet() {
			if (Volume == VOLUME::QUIET)
			{
				return true;
			}
			else
				return false;

		}
		bool IsVeryLoud() {
			if (Volume == VOLUME::VERYLOUD)
			{
				return true;
			}
			else
				return false;
		}

		//Destructor
		virtual ~Rock() {}

		virtual bool operator==(const Rock& r) const {
			return this->MusicType == r.MusicType;
		}

		Rock& operator++(int u) {

			if (Volume == VOLUME::QUIET)
			{
				Volume = VOLUME::LOUD;
			}
			else if (Volume == VOLUME::LOUD)
			{
				Volume = VOLUME::VERYLOUD;
			}
			else if (Volume == VOLUME::VERYLOUD)
			{
				Volume = VOLUME::QUIET;
			}
			else {
			}

			return *this;


		}

		virtual void Play() const
		{
			cout << "Now playing ROCK Music......" << endl;

		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const override
		{
			NonClassical::PrettyPrintClassAttributes(out);
			string volstr = VolumeToString();

			cout << to_string(Expletives) << "," << volstr << endl;

		}

	};

	class Electronic : public NonClassical {

		unsigned int BeatsPerMinute;

	public:


		//Constructor
		Electronic(json& j) : NonClassical(j, MUSIC_TYPE::ELECTRONIC, TEMPO::FAST)
		{
			BeatsPerMinute = j["BeatsPerMin"];


		}

		//Destructor
		virtual ~Electronic() {}

		virtual bool operator==(const Electronic& e) const {
			return this->MusicType == e.MusicType;
		}

		const unsigned int GetBeatsPerMinute() {
			return BeatsPerMinute;
		}

		Electronic& operator--() {
			this->BeatsPerMinute -= 1;
			return *this; // return new value by reference
		}
		Electronic& operator++() {
			// actual increment takes place here
			this->BeatsPerMinute += 1;
			return *this; // return new value by reference
		}

		virtual void Play() const
		{
			cout << "Now playing ELECTRONIC Music......." << endl;

		}

		virtual void PrettyPrintClassAttributes(std::ostream& out = std::cout) const override
		{
			NonClassical::PrettyPrintClassAttributes(out);
			cout << to_string(BeatsPerMinute);
			cout << endl;
		}

	};

	class Machine
	{
		json jf;
		int songCount;
		Music** Songs{};


	public:



		virtual ~Machine() {
			/*destructor
			deallocate all instances of music from heap*/
			for (int i = 0; i < songCount; i++)
				delete Songs[i];

			// deallocate array of pointers to music
			delete[] Songs;

		}

		const Music* GetSong(const int i) {

			//using modulus operator to make sure never greater than songCount
			return Songs[i % songCount];

		}


		Machine(std::string fileName, const int iMaxSongs)
		{
			fstream ifs(fileName);
			jf = json::parse(ifs);
			int songCount = 0;


			// allocate space on heap to store iMaxSongs pointers to Music
			MusicMachine::Machine::Songs = new Music * [iMaxSongs];


			// initialize array of pointers to Music
			for (int i = 0; i < iMaxSongs; i++)
				MusicMachine::Machine::Songs[i] = nullptr;

		}

		Music& operator[](int i) {
			return *(Songs[i % songCount]);
		}

		const size_t Parse()
		{

			if (jf.is_array()) {
				for (json& c : jf)
				{
					//creating instance of each genre on the heap

					if (c["Type"] == "Chamber")
						Songs[songCount++] = new Chamber(c);
					else if (c["Type"] == "Electronic")
						Songs[songCount++] = new Electronic(c);
					else if (c["Type"] == "Jazz")
						Songs[songCount++] = new Jazz(c);
					else if (c["Type"] == "Orchestral")
						Songs[songCount++] = new Orchestral(c);
					else if (c["Type"] == "Pop")
						Songs[songCount++] = new Pop(c);
					else if (c["Type"] == "Rock")
						Songs[songCount++] = new Rock(c);
					else
						cerr << "Unknown class!" << c << endl;
				}

			}

			return songCount;
		}


		const int GetSongCount() const { return songCount; }
		Music** GetSong() const { return Songs; }

	};
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define _CRTDBG_MAP_ALLOC
#include "doctest.h"
#include <crtdbg.h>
#include <stdlib.h>

TEST_CASE("Testing my Music Machine")
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SUBCASE("testing the parsing of file - JsonObject8a.json")
	{
		const int MAX_SONGS = 10;
		MusicMachine::Machine m(DATA_FILE_PATH + "JsonObject8a.json", MAX_SONGS);

		const int MAX_RECORDS = 6;

		//Invoke the parsing process
		CHECK(m.Parse() == MAX_RECORDS);

		const int iSongCount = m.GetSongCount();

		//Notice that the loop is done twice
		for (int i = 0; i < iSongCount * 2; i++) {

			//virtual function of base class "plays" the correct instance of derived class
			m[i].Play();
			cout << m[i] << endl;
			//cout << endl;

			//Test the "==" operator for the music class
			CHECK((m[i] == m[i]) == true);

			//Unit testing the derived classes
			if (m[i].GetMusicType() == MusicMachine::MUSIC_TYPE::CHAMBER)
			{
				MusicMachine::Chamber& c = dynamic_cast<MusicMachine::Chamber&>(m[i]);
				CHECK((c == c) == true);
				CHECK((c).GetKey() == "G");
				CHECK((c++).GetKey() == "A");
				CHECK((c++).GetKey() == "B");
				CHECK((c++).GetKey() == "C");
				CHECK((c++).GetKey() == "D");
				CHECK((c++).GetKey() == "E");
				CHECK((c++).GetKey() == "F");
				CHECK((c++).GetKey() == "G");

			}
			else if (m[i].GetMusicType() == MusicMachine::MUSIC_TYPE::ORCHESTRAL)
			{
				MusicMachine::Orchestral& o = dynamic_cast<MusicMachine::Orchestral&>(m[i]);
				CHECK((o == o) == true);
			}
			else if (m[i].GetMusicType() == MusicMachine::MUSIC_TYPE::JAZZ)
			{
				MusicMachine::Jazz& j = dynamic_cast<MusicMachine::Jazz&>(m[i]);
				CHECK((j == j) == true);
			}
			else if (m[i].GetMusicType() == MusicMachine::MUSIC_TYPE::ROCK)
			{
				MusicMachine::Rock& r = dynamic_cast<MusicMachine::Rock&>(m[i]);
				CHECK((r == r) == true);
				CHECK((r).IsLoud() == true);
				CHECK((r++).IsLoud() == false);
				CHECK((r).IsVeryLoud() == true);
				CHECK((r++).IsVeryLoud() == false);
				CHECK((r).IsQuiet() == true);
				CHECK((r++).IsQuiet() == false);
			}
			else if (m[i].GetMusicType() == MusicMachine::MUSIC_TYPE::ELECTRONIC)
			{
				MusicMachine::Electronic& e = dynamic_cast<MusicMachine::Electronic&>(m[i]);
				CHECK((e == e) == true);
				CHECK((e).GetBeatsPerMinute() == 141);
				CHECK((++e).GetBeatsPerMinute() == 142);
				CHECK((--e).GetBeatsPerMinute() == 141);
			}
		}
	}

	//	SUBCASE("testing the parsing of file - JsonObject7a.json") {
	//		const int MAX_SONGS = 10;
	//		MusicMachine::Machine m(DATA_FILE_PATH + "JsonObject7a.json", MAX_SONGS);
	//
	//		const int MAX_RECORDS = 6;
	//
	//		//Invoke the parsing process
	//		CHECK(m.Parse() == MAX_RECORDS);
	//
	//
	//		const int iSongCount = m.GetSongCount();
	//		for (int i = 0; i < iSongCount; i++) {
	//			//pSong is a pointer to only the base class Music
	//			const MusicMachine::Music* pSong = m.GetSong(i);
	//
	//			//given base class, using c++ rtti "typeinfo" to print name of the derived class
	//			cout << "This is - \"" << typeid(*pSong).name() << "\"" << endl;
	//
	//			//virtual function of base class plays the correct instance of derived class
	//			pSong->Play();
	//			//cout << pSong->PrettyPrintClassAttributes() << endl;
	//			pSong->PrettyPrintClassAttributes();
	//			cout << endl;
	//
	//			// unit tests using just base class
	//			switch (pSong->GetMusicType()) {
	//			case MusicMachine::MUSIC_TYPE::CHAMBER:
	//				CHECK(dynamic_cast<const MusicMachine::Chamber*>(pSong)->IsTempoFast() == false);
	//				break;
	//			case MusicMachine::MUSIC_TYPE::ORCHESTRAL:
	//				CHECK(dynamic_cast<const MusicMachine::Orchestral*>(pSong)->IsTempoFast() == false);
	//				break;
	//			case MusicMachine::MUSIC_TYPE::POP:
	//				CHECK(dynamic_cast<const MusicMachine::Pop*>(pSong)->IsTempoFast() == true);
	//				break;
	//			case MusicMachine::MUSIC_TYPE::JAZZ:
	//				CHECK(dynamic_cast<const MusicMachine::Jazz*>(pSong)->IsTempoFast() == true);
	//				break;
	//			case MusicMachine::MUSIC_TYPE::ROCK:
	//				CHECK(dynamic_cast<const MusicMachine::Rock*>(pSong)->IsTempoFast() == true);
	//				break;
	//			case MusicMachine::MUSIC_TYPE::ELECTRONIC:
	//				CHECK(dynamic_cast<const MusicMachine::Electronic*>(pSong)->IsTempoFast() == true);
	//				break;
	//			default:
	//				cerr << "Error determining music type!" << endl;
	//			}
	//		}
	//	}
	//
	//	SUBCASE("testing the parsing of file - JsonObject6a.json")
	//	{
	//		const int MAX_SONGS = 10;
	//		MusicMachine::Machine m(DATA_FILE_PATH + "JsonObject6a.json", MAX_SONGS);
	//		// Invoke the parsing process
	//		CHECK(m.Parse() == 6);
	//
	//		/// Chamber
	//		vector<MusicMachine::Chamber> ChamberSongs = m.GetChamberSongs();
	//		for (auto& cs : ChamberSongs) {
	//			cs.Play();
	//			cout << cs << endl;
	//			CHECK(cs.GetMusicType() == MusicMachine::MUSIC_TYPE::CHAMBER);
	//			CHECK(cs.IsTempoFast() == false);
	//		}
	//
	//		/// Orchestral
	//		vector<MusicMachine::Orchestral> OrchestralSongs = m.GetOrchestralSongs();
	//		for (auto& cs : OrchestralSongs) {
	//			cs.Play();
	//			cout << cs << endl;
	//			CHECK(cs.GetMusicType() == MusicMachine::MUSIC_TYPE::ORCHESTRAL);
	//			CHECK(cs.IsTempoFast() == false);
	//		}
	//
	//		/// Pop
	//		vector<MusicMachine::Pop> PopSongs = m.GetPopSongs();
	//		for (auto& cs : PopSongs) {
	//			cs.Play();
	//			cout << cs << endl;
	//			CHECK(cs.GetMusicType() == MusicMachine::MUSIC_TYPE::POP);
	//			CHECK(cs.IsTempoFast() == true);
	//		}
	//
	//		/// Jazz
	//		vector<MusicMachine::Jazz> JazzSongs = m.GetJazzSongs();
	//		for (auto& cs : JazzSongs) {
	//			cs.Play();
	//			cout << cs << endl;
	//			CHECK(cs.GetMusicType() == MusicMachine::MUSIC_TYPE::JAZZ);
	//			CHECK(cs.IsTempoFast() == true);
	//
	//			/// Rock
	//			vector<MusicMachine::Rock> RockSongs = m.GetRockSongs();
	//			for (auto& cs : RockSongs) {
	//				cs.Play();
	//				cout << cs << endl;
	//				CHECK(cs.GetMusicType() == MusicMachine::MUSIC_TYPE::ROCK);
	//				CHECK(cs.IsTempoFast() == true);
	//			}
	//
	//			/// Electronic
	//			vector<MusicMachine::Electronic> ElectronicSongs = m.GetElectronicSongs();
	//			for (auto& cs : ElectronicSongs) {
	//				cs.Play();
	//				cout << cs << endl;
	//				CHECK(cs.GetMusicType() == MusicMachine::MUSIC_TYPE::ELECTRONIC);
	//				CHECK(cs.IsTempoFast() == true);
	//			}
	//		}
	//	}

};

std::ostream& operator<<(std::ostream& out, const MusicMachine::Music& ex_machina) {
	ex_machina.PrettyPrintClassAttributes(out);
	return out;
}