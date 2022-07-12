# MusicMachine
![msbuild](https://github.com/ITCS-2550/musicmachine-teama/actions/workflows/msbuild.yml/badge.svg)

The shell of the MusicMachine project.

In this program we will define a namespace called "MusicMachine"
Within the namespace MusicMachine we will have:
Machine Class
		Parse through JSON file and store data in appropriate vector
Music Class 
		Act as the base class for all other music types
			Has functions for:
				GetMusicType
				IsTempoFast
				Play
				PrettyPrintClassAttributes
			Has enum for Tempo
			Classical Class:
				Chamber
					-> Has Instrument Struct (One, Two, Three, Four)
				Orchestral
					-> Has Instrument Vector
			NonClassical Class:
				Pop
					->DateofRanking
					->Top 40 Ranking
				Jazz
					->enum for Improvisation
					->ImprovisationToString()
				Rock
					->Expletives
					->GuitarsOnly
					->enum for Volume
					->VolumeToString
				Electronic