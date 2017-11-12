#pragma once

#include "record.h"
#include "spectogram.h"
#include <iostream>
#include <algorithm>



#define string_dir_to_test_files "/home/keerthikan/soundcloud/src/include/database/test_files/"

class database {
public:
    vector<AudioFile<SAMPLE>> list_of_files;
    database();

    void do_stuff();
};
