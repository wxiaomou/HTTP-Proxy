// File:        LogUtils.h
// Author:      zhaohang, wxiaomou, EMFISH
// Description: This File provides several method for recording logs
#ifndef _LOGUTILS_H
#define _LOGUTILS_H

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sstream>
using namespace std;


// If set this flag true, the program will print logs correspond to the author
static bool zhaohang_log = false;
static bool wxiaomou_log = true;
static bool emfish_log = false;

static bool error_log = true;

// Print error, example: error("dequeue failure");
static void error(const char *msg) {
    if (error_log)
	cerr << msg << endl;
    exit(0);
}

// Print logs for DEBUG (set the flag false when submitting the code)
// tag is the author's name("zhaohang", "wxiaomou", "EMFISH"), msg is the debug message
// Example: log_d("zhaohang", "enqueue error");
// or log_d("EMFISH", string("value = ") + to_string(v));
static void log_d(const char *tag, const string msg) {

    // Output the log
    if (strcmp(tag, "zhaohang") == 0 && zhaohang_log) {
	cout << tag << ": " << msg << endl; 
	return;
    }
    if (strcmp(tag, "wxiaomou") == 0 && wxiaomou_log) {
	cout << tag << ": " << msg << endl; 
	return;
    }
    if (strcmp(tag, "emfish") == 0 && emfish_log) {
	cout << tag << ": " << msg << endl; 
	return;
    }
}

// Convert int to string
static string to_string(int i) {
    std::stringstream out;
    out << i;
    return out.str();
}

#endif


