#define BOOST_TEST_MODULE WavefrontObjTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "CE3D2/tests/TestUtilities.h"
#include "CE3D2/models/loaders/WavefrontObj.h"

#include <boost/filesystem.hpp>

std::string get_testfile_path(std::string testfile) {
    boost::filesystem::path current_file_path(__FILE__);
    boost::filesystem::path test_directory = current_file_path.stem();
    test_directory += "Files";
    boost::filesystem::path testfile_path(testfile);
    return (test_directory / testfile_path).string();
}

BOOST_AUTO_TEST_SUITE(WavefrontObjTestSuite)

    // TODO
    BOOST_AUTO_TEST_CASE(test_simple)
    {
        auto models = CE3D2::Models::Loaders::load_wavefront_obj(get_testfile_path("simple.obj"));

        BOOST_CHECK_EQUAL(models.size(), 1);

        auto const& model = *models[0];
        auto expected = CE3D2::Models::LineModel("a_cube");
        expected.vectors().push_back(CE3D2::create_vector(1.0f, 1.0f, -1.0f));
        expected.vectors().push_back(CE3D2::create_vector(2.0f, -2.0f, 0.5f));
        expected.vectors().push_back(CE3D2::create_vector(4.0f, 10.0f, -3.0f));

        CE3D2_CHECK_MODELS_EQUAL(model, expected);

        //expected.connections().push_back(CE3D2::Models::IndexPair());
    }

BOOST_AUTO_TEST_SUITE_END()
