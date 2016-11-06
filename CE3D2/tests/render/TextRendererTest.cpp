#define BOOST_TEST_MODULE TextRendererTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "CE3D2/render/TextRenderer.h"
#include "CE3D2/tests/TestUtilities.h"
#include "CE3D2/transformation/OrthogonalProjection.h"
#include "CE3D2/transformation/Scale.h"
#include "CE3D2/transformation/Translation.h"


BOOST_AUTO_TEST_SUITE(TextRendererTestSuite)

BOOST_AUTO_TEST_CASE(test_rendered_char)
{
    CE3D2::Render::TextRenderer uut;

    BOOST_CHECK_EQUAL(uut.get_rendered_char(), '.');

    uut.set_rendered_char('x');

    BOOST_CHECK_EQUAL(uut.get_rendered_char(), 'x');
}

BOOST_AUTO_TEST_CASE(test_target)
{
    CE3D2::Render::TextRenderer uut;
    std::shared_ptr<CE3D2::Render::TextSurface> surf(
        new CE3D2::Render::TextSurface(10, 10));

    BOOST_CHECK_EQUAL(uut.get_target().get(),
                      (CE3D2::Render::TextSurface*)nullptr);

    uut.set_target(surf);

    BOOST_CHECK_EQUAL(uut.get_target(), surf);

    (*surf)(3, 8) = 'T';

    BOOST_CHECK_EQUAL((*uut.get_target())(3, 8), 'T');

    // Test modifiability.
    (*uut.get_target())(7, 7) = 'W';
    BOOST_CHECK_EQUAL((*surf)(7, 7), 'W');
}

BOOST_AUTO_TEST_CASE(test_models)
{
    CE3D2::Render::TextRenderer uut;
    std::shared_ptr<CE3D2::Models::Model> model(
        new CE3D2::Models::Model("myModel"));

    BOOST_CHECK(uut.models().empty());

    uut.models().push_back(model);

    BOOST_CHECK_EQUAL(uut.models().size(), 1);
    BOOST_CHECK_EQUAL(uut.models()[0]->get_name(), "myModel");

    model->set_name("new_name");

    BOOST_CHECK_EQUAL(uut.models()[0]->get_name(), "new_name");
}

BOOST_AUTO_TEST_CASE(test_line_models)
{
    CE3D2::Render::TextRenderer uut;
    std::shared_ptr<CE3D2::Models::LineModel> model(
        new CE3D2::Models::LineModel("myModel"));

    BOOST_CHECK(uut.line_models().empty());

    uut.line_models().push_back(model);

    BOOST_CHECK_EQUAL(uut.line_models().size(), 1);
    BOOST_CHECK_EQUAL(uut.line_models()[0]->get_name(), "myModel");

    model->set_name("new_name");

    BOOST_CHECK_EQUAL(uut.line_models()[0]->get_name(), "new_name");
}

BOOST_AUTO_TEST_CASE(test_render_without_target)
{
    CE3D2::Render::TextRenderer uut;
    BOOST_CHECK_THROW(uut.render(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_render)
{
    auto surface = std::make_shared<CE3D2::Render::TextSurface>(10, 5);

    auto model1 = std::make_shared<CE3D2::Models::Model>();
    model1->vectors().push_back(CE3D2::create_vector(1.0f, 1.0f));
    model1->vectors().push_back(CE3D2::create_vector(4.0f, 5.0f));
    model1->vectors().push_back(CE3D2::create_vector(22.0f, 3.0f));
    model1->vectors().push_back(CE3D2::create_vector(5.7f, 0.1f));
    model1->vectors().push_back(CE3D2::create_vector(-2.0f, 3.3f));
    model1->vectors().push_back(CE3D2::create_vector(0.0f, 4.99f));

    CE3D2::Render::TextRenderer uut;
    uut.set_target(surface);
    uut.models().push_back(model1);

    uut.render();

    auto expected_surface = CE3D2_CREATE_TEXTSURFACE("     .    ",
                                                     " .        ",
                                                     "          ",
                                                     "          ",
                                                     ".         ");
    CE3D2_CHECK_TEXTSURFACES_EQUAL(*surface, *expected_surface);

    auto model2 = std::make_shared<CE3D2::Models::Model>("named model");
    model2->vectors().push_back(CE3D2::create_vector(9.4f, 4.0f));
    model2->vectors().push_back(CE3D2::create_vector(9.2f, 3.0f));
    model2->vectors().push_back(CE3D2::create_vector(9.0f, 2.0f));
    model2->vectors().push_back(CE3D2::create_vector(8.999f, 1.0f));

    uut.models().push_back(model2);
    uut.set_rendered_char('o');
    surface->clear();
    uut.render();

    expected_surface = CE3D2_CREATE_TEXTSURFACE("     o    ",
                                                " o      o ",
                                                "         o",
                                                "         o",
                                                "o        o");
    CE3D2_CHECK_TEXTSURFACES_EQUAL(*surface, *expected_surface);

    model1->set_visibility(false);
    uut.render();

    // The result should be like before, we need to clear the target surface
    // manually!
    CE3D2_CHECK_TEXTSURFACES_EQUAL(*surface, *expected_surface);

    surface->clear();
    uut.render();

    expected_surface = CE3D2_CREATE_TEXTSURFACE("          ",
                                                "        o ",
                                                "         o",
                                                "         o",
                                                "         o");
    CE3D2_CHECK_TEXTSURFACES_EQUAL(*surface, *expected_surface);

    model2->set_visibility(false);
    surface->clear();
    uut.render();

    expected_surface = CE3D2_CREATE_TEXTSURFACE("          ",
                                                "          ",
                                                "          ",
                                                "          ",
                                                "          ");
    CE3D2_CHECK_TEXTSURFACES_EQUAL(*surface, *expected_surface);
}

BOOST_AUTO_TEST_CASE(test_line_render)
{
    // As the result is subjective, we fix a certain result so we can track/test
    // changes to the algorithm affecting line-rendering. Also we can unreveal
    // bugs, especially segmentation faults.

    auto cube = std::make_shared<CE3D2::Models::LineModel>(
        CE3D2::Models::LineModel::cube());
    cube->transform(CE3D2::Transformation::Scale(
        CE3D2::ScalarVector(3, 10.0f)));

    CE3D2::Transformation::OrthogonalProjection ortho_projection;
    std::vector<CE3D2::Vector> projection_vecs;
    projection_vecs.push_back(CE3D2::create_vector(0.7f, 0.3f, 0.0f));
    projection_vecs.push_back(CE3D2::create_vector(0.0f, 0.3f, 0.7f));
    ortho_projection.set_projection_vectors(projection_vecs);

    cube->transform(ortho_projection);
    cube->transform(CE3D2::Transformation::Translation(
        CE3D2::create_vector(10.0f, 10.0f)));

    CE3D2::Render::TextRenderer renderer;
    auto surface = std::make_shared<CE3D2::Render::TextSurface>(22, 20);

    renderer.set_target(surface);
    renderer.line_models().push_back(cube);
    renderer.render();

    auto expected_surface = CE3D2_CREATE_TEXTSURFACE(
        "          _____       ",
        "   ______/     |      ",
        "               |\\     ",
        "  |\\           / \\    ",
        "  | \\         |  _\\   ",
        "  |  \\     ___|_/  |  ",
        "  |   \\___/   |    |  ",
        "  |    |      |    |  ",
        "  |    |      |    |  ",
        "  /    |      /    |  ",
        " |     /     |     /  ",
        " |    |      |    |   ",
        " |    |   ___|    |   ",
        " |  __|__/   \\    |   ",
        " |_/  |       \\   |   ",
        " \\    |        \\  |   ",
        "  \\   /         \\ |   ",
        "   \\ |     ______     ",
        "    \\|____/           ",
        "                      ");

    CE3D2_CHECK_TEXTSURFACES_EQUAL(*surface, *expected_surface);
}

BOOST_AUTO_TEST_SUITE_END()
