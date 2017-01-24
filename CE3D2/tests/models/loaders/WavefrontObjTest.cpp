#define BOOST_TEST_MODULE WavefrontObjTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include "CE3D2/models/loaders/WavefrontObj.h"


std::string get_testfile_path(std::string testfile) {
    boost::filesystem::path current_file_path(__FILE__);
    boost::filesystem::path test_directory = current_file_path.stem();
    boost::filesystem::path testfile_path(testfile);
    return (test_directory / testfile_path).string();
}


#define CE3D2_CHECK_TEXTSURFACES_EQUAL(A, B) \
    CE3D2_ASSERT_TEXTSURFACES_EQUAL(A, B, CHECK)

// TODO Part up connection check into ASSERT_INDEX_PAIR_EQUAL
#define CE3D2_ASSERT_MODELS_EQUAL(a, b, LEVEL)                                          \
{                                                                                       \
        BOOST_##LEVEL##_EQUAL((a).get_name(), (b).get_name());                    \
        BOOST_##LEVEL##_EQUAL((a).is_visible(), (b).is_visible());                \
                                                                                        \
        auto const& vectors_a = (a).vectors();                                          \
        auto const& vectors_b = (b).vectors();                                          \
        BOOST_##LEVEL##_EQUAL(vectors_a.size(), vectors_b.size());                      \
        for (decltype(vectors_a)::size_type k = 0; k < vectors_a.size(); k++)           \
        {                                                                               \
            CE3D2_ASSERT_VECTORS_EQUAL(vectors_a[k], vectors_b[k], LEVEL);              \
        }                                                                               \
                                                                                        \
        auto const& connections_a = (a).connections();                                  \
        auto const& connections_b = (b).connections();                                  \
                                                                                        \
        BOOST_##LEVEL##_EQUAL(connections_a.size(), connections_b.size());              \
        for (decltype(connections_a)::size_type k = 0; k < connections_a.size(); k++)   \
        {                                                                               \
            auto const& connection_a = connections_a[k];                                \
            auto const& connection_b = connections_b[k];                                \
                                                                                        \
            bool connection_is_equal = connection_a.first == connection_b.first &&      \
                                       connection_a.second == connection_b.second;      \
                                                                                        \
            BOOST_##LEVEL##_MESSAGE(connection_is_equal,                                \
                "Connection differs in element " << k << " ("                           \
                << connection_a.first << ", " << connection_a.second                    \
                << " != " <<                                                            \
                connection_b.first << ", " << connection_b.second << ")." )             \
        }                                                                               \
    }                                                                                   \
}


BOOST_AUTO_TEST_SUITE(WavefrontObjTestSuite)



    // TODO
    BOOST_AUTO_TEST_CASE(test_simple)
    {
        auto models = CE3D2::Models::Loaders::load_wavefront_obj(get_testfile_path("simple.obj"));

        BOOST_CHECK_EQUAL(models.size(), 1);
        auto const& model = *models[0];
        BOOST_CHECK_EQUAL(model.get_name(), "a_cube");
        model.connections().size()
        BOOST_CHECK_EQUAL(model.get_name(), "a_cube");
    }

BOOST_AUTO_TEST_SUITE_END()
