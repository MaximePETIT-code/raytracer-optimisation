add_executable(raytracer_tests test_raytracer.cpp)

target_include_directories(raytracer_tests PUBLIC
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           "${PROJECT_SOURCE_DIR}/src/lodepng"
                           )

target_link_libraries(raytracer_tests 
                      PUBLIC 
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      Threads::Threads
                      )

add_test(NAME RenderTestTwoSpheresOnPlane COMMAND raytracer_tests /app/scenes/two-spheres-on-plane.json /app/test/reference-two-spheres-on-plane.png /app/test/test-output-two-spheres-on-plane.png)
add_test(NAME RenderTestManySpheresOnPlan COMMAND raytracer_tests /app/scenes/sphere-galaxy-on-plane.json /app/test/reference-many-spheres-on-plane.png /app/test/test-output-many-spheres-on-plane.png)
# add_test(NAME RenderTestIsoSphereOnPlane COMMAND raytracer_tests /app/scenes/iso-sphere-on-plane.json /app/test/reference-iso-sphere-on-plane.png /app/test/test-output-iso-sphere-on-plane.png)