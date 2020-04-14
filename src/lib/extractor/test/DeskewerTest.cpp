#include "unittest.h"

#include "Deskewer.h"
#include <iostream>
#include <string>
#include <vector>

std::string get_sample(std::string filename)
{
	return std::string(LIBCIMBAR_PROJECT_ROOT) + "/samples/" + filename;
}

TEST_CASE( "DeskewerTest/testSimple", "[unit]" )
{
	Corners corners({169, 981}, {2409, 897}, {2343, 3066}, {324, 3030});
	Deskewer de;

	cv::Mat actual = de.deskew(get_sample("4color1.jpg"), corners);
	assertEquals(cv::Size(1024, 1024), actual.size());

	// TODO: we'll want to do some verification that the transform worked correctly (perhaps with an imagehash??)
	// but for now, this is fine
	de.save(actual, "/tmp/foo.png");
}
