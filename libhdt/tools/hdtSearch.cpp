/*
 * File: HDTEnums.cpp
 * Last modified: $Date: 2012-08-13 23:00:07 +0100 (lun, 13 ago 2012) $
 * Revision: $Revision: 222 $
 * Last modified by: $Author: mario.arias $
 *
 * Copyright (C) 2012, Mario Arias, Javier D. Fernandez, Miguel A. Martinez-Prieto
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Contacting the authors:
 *   Mario Arias:               mario.arias@gmail.com
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *
 */
#include <iomanip>

#include <HDTVersion.hpp>
#include <HDT.hpp>
#include <HDTManager.hpp>
#include <signal.h>

#include <getopt.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "../src/util/StopWatch.hpp"

using namespace hdt;
using namespace std;

StopWatch st;

int interruptSignal = 0;

void signalHandler(int sig)
{
	interruptSignal = 1;
}

void help() {
	cout << "$ hdtSearch [options] <hdtfile> " << endl;
	cout << "\t-h\t\t\tThis help" << endl;
	cout << "\t-q\t<query>\t\tLaunch query and exit." << endl;
	cout << "\t-i\t<input>\t\tLaunch queries from input file." << endl;
	cout << "\t-o\t<output>\tSave query output to file." << endl;
    cout << "\t-f\t<offset>\tLimit the result list starting after the offset." << endl;
	cout << "\t-m\t\t\tDo not show results, just measure query time." << endl;
	cout << "\t-V\t\t\tPrints the HDT version number." << endl;
	//cout << "\t-v\tVerbose output" << endl;
}

void iterate(HDT *hdt, char *query, ostream &out, bool measure, uint32_t offset) {
	TripleString tripleString;
	tripleString.read(query);

	const char *subj = tripleString.getSubject().c_str();
	const char *pred = tripleString.getPredicate().c_str();
	const char *obj = tripleString.getObject().c_str();
	if(strcmp(subj, "?")==0) {
		subj="";
	}
	if(strcmp(pred, "?")==0) {
		pred="";
	}
	if(strcmp(obj, "?")==0) {
		obj="";
	}

#if 0
	cout << "Subject: |" << subj <<"|"<< endl;
	cout << "Predicate: |" << pred <<"|"<< endl;
	cout << "Object: |" << obj << "|"<<endl;
#endif

	try {
		IteratorTripleString *it = hdt->search(subj, pred, obj);

		st.reset();

        // Go to the right offset.
        if(it->canGoTo()) {
            try {
                it->skip(offset);
                offset = 0;
            }
            catch (const runtime_error error) {
                /*invalid offset*/
                interruptSignal = 1;
            }
        }
        else {
            while(offset && it->hasNext()) {
                it->next();
                offset--;
            }
        }

        // Get results.
		unsigned int numTriples=0;
		while(it->hasNext() && interruptSignal==0) {
			TripleString *ts = it->next();
			if(!measure)
				out << *ts << endl;
			numTriples++;
		}
		cerr << numTriples << " results in " << st.stopReal() << endl;
		delete it;

		interruptSignal=0;	// Interrupt caught, enable again.
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}

}

int main(int argc, char **argv) {
	int c;
	string query, inputFile, outputFile, queryFile;
    stringstream sstream;
    uint32_t offset = 0;
	bool measure = false;

	while( (c = getopt(argc,argv,"hq:i:o:f:mV"))!=-1) {
		switch(c) {
		case 'h':
			help();
			break;
		case 'q':
			query = optarg;
			break;
		case 'i':
			queryFile = optarg;
			break;
		case 'o':
			outputFile = optarg;
			break;
        case 'f':
            sstream << optarg;
            if(!(sstream >> offset)) offset=0;
            break;
		case 'm':
			measure = true;
			break;
		case 'V':
			cout << HDTVersion::get_version_string(".") << endl;
			return 0;
		default:
			cerr << "ERROR: Unknown option" << endl;
			help();
			return 1;
		}
	}

	if(argc-optind<1) {
		cerr << "ERROR: You must supply an HDT File" << endl << endl;
		help();
		return 1;
	}

	inputFile = argv[optind];


	try {
		StdoutProgressListener prog;
		st.reset();
		HDT *hdt = HDTManager::mapIndexedHDT(inputFile.c_str(), &prog);
		cerr << endl << " HDT loaded in " << st.stopReal() << endl;

		ostream *out;
		ofstream outF;

		if(outputFile!="") {
			outF.open(outputFile.c_str());
			out = &outF;
		} else {
			out = &cout;
		}

		if(queryFile!="") {
			ifstream queryStream(queryFile);
			string line;
			while (getline(queryStream,line))
			{
				char linechar[1024*10];
				strcpy(linechar, line.c_str());
				iterate(hdt, linechar, *out, measure, offset);
			}
		} else if(query!="") {
			// Supplied query, search and exit.
			iterate(hdt, (char*)query.c_str(), *out, measure, offset);
		} else {
			// No supplied query, show terminal.
			char line[1024*10];

			signal(SIGINT, &signalHandler);
			cerr << "                                                 \r>> ";
			while(cin.getline(line, 1024*10)) {
				if(strcmp(line, "exit")==0|| strcmp(line,"quit")==0) {
					break;
				}
				if(strlen(line)==0 || strcmp(line, "help")==0) {
					cerr << "Please type Triple Search Pattern, using '?' for wildcards. e.g " << endl;
					cerr << "   http://www.somewhere.com/mysubject ? ?" << endl;
					cerr << "Interrupt with Control+C. Type 'exit', 'quit' or Control+D to exit the shell." << endl;
					cerr << ">> ";
					continue;
				}

				iterate(hdt, line, *out, measure, offset);

				cerr << ">> ";
			}
		}

		if(outputFile!="") {
			outF.close();
		}

		delete hdt;
	} catch (std::exception& e) {
		cerr << "ERROR: " << e.what() << endl;
		return 1;
	}
}
