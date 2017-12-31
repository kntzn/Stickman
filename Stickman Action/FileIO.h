#pragma once
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

void loadFromFile (const char filename [], char* &buffer, size_t &bufSize, bool replaceSpc = false, bool output = false)
	{
	unsigned long int nBytes = 0; // File Size
	struct stat fileStat; // Stat class to find file's size

	stat (filename, &fileStat);
	if (output)
		printf ("%d bytes found\n", fileStat.st_size);

	unsigned long int bytesToLoad = fileStat.st_size;
	buffer = (char*) calloc (bytesToLoad, sizeof (int));

	// Opening file
	int fh;
	_sopen_s (&fh, filename, _O_RDONLY, _SH_DENYNO, 0);
	if (fh == -1)
		{
		printf ("Failed to open %s", "output.txt");
		perror ("");
		exit (1);
		}

	// Reading file
	if ((nBytes = _read (fh, buffer, bytesToLoad)) == -1)
		perror ("Unexpected error loading the file");		
	else if (output)
		printf ("Read %u bytes from file\n", nBytes);
	
	bufSize = nBytes;

	//Replace
	for (unsigned long int i = 0; i < nBytes; i++)
		{
		if (replaceSpc)
			if (buffer [i] == ' ')
				buffer [i] = '\0';
		}

	// Closing file
	_close (fh);
	}

void saveToFile (char filename [], char* buffer, size_t bufSize, bool output = false)
	{
	// Opening file
	int fh;
	_sopen_s (&fh, filename, _O_RDWR | _O_CREAT, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	if (!(fh))
		exit (-1);

	// Writing buffer's data to file
	unsigned int writtenToFile = _write (fh, buffer, bufSize);
	if (writtenToFile == -1)
		perror ("Unexpected error saving the file");
	else if (output)
		printf ("Wrote %u bytes to file.\n", writtenToFile);

	// Closing File
	_close (fh);
	}