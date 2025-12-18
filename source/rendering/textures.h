#ifndef CIV__textures
#define CIV__textures

/* Include */
// base
#include "../basic.h"
#include "../random/random.h"

// shaders
#include "shaders.h"

/* Define */
// types
typedef BASIC__u8 TEX__pixel_color;
typedef BASIC__u16 TEX__dimension_length;
typedef BASIC__u16 TEX__face_count;
typedef TEX__face_count TEX__face_index;
typedef BASIC__u64 TEX__pixel_index;

// values
#define TEX__pixel_colors_count 4
#define TEX__tile_faces__texture_unit_ID 0
#define TEX__tile_faces__texture_ID (GL_TEXTURE0 + TEX__tile_faces__texture_unit_ID)
#define TEX__tile_faces__texture_type GL_TEXTURE_2D_ARRAY
#define TEX__tile_faces__faces_per_tile 6

/* Pixel */
// one pixel
typedef struct TEX__pixel {
    TEX__pixel_color red;
    TEX__pixel_color green;
    TEX__pixel_color blue;
    TEX__pixel_color alpha;
} TEX__pixel;

// create custom pixel
TEX__pixel TEX__create__pixel(TEX__pixel_color red, TEX__pixel_color green, TEX__pixel_color blue, TEX__pixel_color alpha) {
    TEX__pixel output;

    // setup output
    output.red = red;
    output.green = green;
    output.blue = blue;
    output.alpha = alpha;

    return output;
}

// create null pixel
TEX__pixel TEX__create_null__pixel() {
    // return null pixel
    return TEX__create__pixel(0, 0, 0, 0);
}

/* Texture Faces */
typedef struct TEX__faces {
    BASIC__buffer faces;
    TEX__dimension_length width;
    TEX__dimension_length height;
    TEX__face_count count;
} TEX__faces;

// create custom faces struct
TEX__faces TEX__create__faces(BASIC__buffer faces, TEX__dimension_length width, TEX__dimension_length height, TEX__face_count count) {
    TEX__faces output;

    // setup output
    output.faces = faces;
    output.width = width;
    output.height = height;
    output.count = count;

    return output;
}

// create null faces
TEX__faces TEX__create_null__faces() {
    // return null
    return TEX__create__faces(BASIC__create_null__buffer(), 0, 0, 0);
}

// destroy faces
void TEX__destroy__faces(TEX__faces faces) {
    // deallocate
    BASIC__close__buffer(faces.faces);

    return;
}

// open faces
TEX__faces TEX__open__faces(TEX__dimension_length width, TEX__dimension_length height, TEX__face_count face_count) {
    TEX__faces output;

    // setup output
    output.faces = BASIC__open__buffer(sizeof(TEX__pixel) * width * height * face_count);
    output.width = width;
    output.height = height;
    output.count = face_count;

    return output;
}

/* All Game Textures */
// game texture type
typedef enum TEX__gtt {
    // game texture types
    TEX__gtt__tile_faces,
    TEX__gtt__LIMIT,

    // count
    TEX__gtt__COUNT = TEX__gtt__LIMIT,
} TEX__gtt;

// game textures
typedef struct TEX__game_textures {
    TEX__faces tile_faces;
    GLuint tile_faces_handle;
} TEX__game_textures;

// create game textures
TEX__game_textures TEX__create__game_textures(TEX__faces tile_faces, GLuint tile_faces_handle) {
    TEX__game_textures output;

    // setup output
    output.tile_faces = tile_faces;
    output.tile_faces_handle = tile_faces_handle;

    return output;
}

// create null game textures
TEX__game_textures TEX__create_null__game_textures() {
    // return null
    return TEX__create__game_textures(TEX__create_null__faces(), 0);
}

// bind a specific texture buffer to be used in rendering
void TEX__bind__game_textures__specific(TEX__game_textures game_textures, TEX__gtt game_texture_type, SHADER__program shader_program) {
    // bind according to type
    switch (game_texture_type) {
        case TEX__gtt__tile_faces:
            // set active texture (one per game texture type)
            glActiveTexture(TEX__tile_faces__texture_ID);

            // bind texture buffer
            glBindTexture(TEX__tile_faces__texture_type, game_textures.tile_faces_handle);

            // set active texture unit in opengl shader uniform
            glUniform1i(glGetUniformLocation(shader_program.program_ID, "GLOBAL_current_texture_unit"), TEX__tile_faces__texture_unit_ID);

            break;
        default:
            break;
    }

    return;
}

// unbind a specific texture buffer to be used in rendering
void TEX__unbind__game_textures__specific(TEX__game_textures game_textures, TEX__gtt game_texture_type) {
    // quiet compiler warning
    game_textures = game_textures;

    // unbind according to type
    switch (game_texture_type) {
        case TEX__gtt__tile_faces:
            // unbind texture buffer
            glBindTexture(TEX__tile_faces__texture_type, 0);
            
            break;
        default:
            break;
    }

    return;
}

// setup game textures on the cpu and gpu
TEX__game_textures TEX__open__game_textures(TEX__faces tile_faces, SHADER__program shader_program) {
    TEX__game_textures output;

    // setup cpu side data for tile faces
    output.tile_faces = tile_faces;
    glCreateTextures(TEX__tile_faces__texture_type, 1, &(output.tile_faces_handle));

    // setup opengl side for tile faces
    TEX__bind__game_textures__specific(output, TEX__gtt__tile_faces, shader_program);
    glTexParameteri(TEX__tile_faces__texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(TEX__tile_faces__texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(TEX__tile_faces__texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(TEX__tile_faces__texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage3D(TEX__tile_faces__texture_type, 0, GL_RGBA8, tile_faces.width, tile_faces.height, tile_faces.count, 0, GL_RGBA /* Do not change to GL_RGBA8, crashes */, GL_UNSIGNED_BYTE, tile_faces.faces.start);
    TEX__unbind__game_textures__specific(output, TEX__gtt__tile_faces);

    return output;
}

// close the game textures on the cpu and gpu
void TEX__close__game_textures(TEX__game_textures game_textures) {
    // close opengl handles
    glDeleteTextures(1, &(game_textures.tile_faces_handle));

    // destroy cpu data
    TEX__destroy__faces(game_textures.tile_faces);

    return;
}

/* Generating Textures */
// calculate write to pointer in faces
BASIC__address TEX__calculate__faces_pointer(TEX__faces faces, TEX__face_index face_index) {
    // return calculation
    return faces.faces.start + (sizeof(TEX__pixel) * faces.width * faces.height * face_index);
}

// write one pixel
BASIC__address TEX__write__pixel(BASIC__address output_faces_pointer, TEX__pixel pixel) {
    // write red pixel
    *((TEX__pixel_color*)output_faces_pointer) = pixel.red;
    output_faces_pointer += sizeof(TEX__pixel_color);

    // write green pixel
    *((TEX__pixel_color*)output_faces_pointer) = pixel.green;
    output_faces_pointer += sizeof(TEX__pixel_color);
    
    // write blue pixel
    *((TEX__pixel_color*)output_faces_pointer) = pixel.blue;
    output_faces_pointer += sizeof(TEX__pixel_color);
    
    // write alpha pixel
    *((TEX__pixel_color*)output_faces_pointer) = pixel.alpha;
    output_faces_pointer += sizeof(TEX__pixel_color);

    return output_faces_pointer;
}

// create a solid colored face
void TEX__generate_face__one_color(TEX__faces faces, TEX__face_index face_index, TEX__pixel pixel) {
    BASIC__address write_to;

    // get write to pointer
    write_to = TEX__calculate__faces_pointer(faces, face_index);

    // write data
    for (TEX__face_index width = 0; width < faces.width; width++) {
        for (TEX__face_index height = 0; height < faces.height; height++) {
            // write pixel
            write_to = TEX__write__pixel(write_to, pixel);
        }
    }

    return;
}

// create a semi randomly colored face
void TEX__generate_face__one_color_range(TEX__faces faces, TEX__face_index face_index, TEX__pixel initial_pixel, RANDOM__context* random_context, TEX__pixel_color color_intensity) {
    BASIC__address write_to;
    TEX__pixel temp_pixel;

    // get write to pointer
    write_to = TEX__calculate__faces_pointer(faces, face_index);

    // write pixels
    for (TEX__face_index width = 0; width < faces.width; width++) {
        for (TEX__face_index height = 0; height < faces.height; height++) {
            // create pixel
            temp_pixel = initial_pixel;

            // randomize colors
            temp_pixel.red += (RANDOM__generate_number_and_advance__mark_1(random_context) % color_intensity);
            temp_pixel.green += (RANDOM__generate_number_and_advance__mark_1(random_context) % color_intensity);
            temp_pixel.blue += (RANDOM__generate_number_and_advance__mark_1(random_context) % color_intensity);

            // write pixel
            write_to = TEX__write__pixel(write_to, temp_pixel);
        }
    }

    return;
}

// create a box shaped texture
void TEX__generate_face__box_texture(TEX__faces faces, TEX__face_index face_index, TEX__pixel border, TEX__pixel center) {
    BASIC__address write_to;

    // get write to pointer
    write_to = TEX__calculate__faces_pointer(faces, face_index);

    // write top row
    for (TEX__pixel_index width = 0; width < faces.width; width++) {
        // write pixel
        write_to = TEX__write__pixel(write_to, border);
    }

    // write body
    for (TEX__pixel_index height = 1; height < (BASIC__u64)faces.height - 1; height++) {
        // write left wall
        write_to = TEX__write__pixel(write_to, border);

        // write center
        for (TEX__pixel_index width = 1; width < (BASIC__u64)faces.width - 1; width++) {
            // write center
            write_to = TEX__write__pixel(write_to, center);
        }

        // write right wall
        write_to = TEX__write__pixel(write_to, border);
    }

    // write bottom row
    for (TEX__pixel_index width = 0; width < faces.width; width++) {
        // write pixel
        write_to = TEX__write__pixel(write_to, border);
    }

    return;
}

// create a checkerboard styled texture
void TEX__generate_face__checkerboard(TEX__faces faces, TEX__face_index face_index, TEX__pixel a, TEX__pixel b) {
    BASIC__address write_to;

    // get write to pointer
    write_to = TEX__calculate__faces_pointer(faces, face_index);

    // draw each double row
    for (TEX__pixel_index row = 0; row < faces.height; row += 2) {
        // draw the first row
        for (BASIC__u64 pixel = 0; pixel < faces.width; pixel++) {
            if (pixel % 2 == 0) {
                write_to = TEX__write__pixel(write_to, a);
            } else {
                write_to = TEX__write__pixel(write_to, b);
            }
        }
        // draw the second row
        for (BASIC__u64 pixel = 0; pixel < faces.width; pixel++) {
            if (pixel % 2 == 1) {
                write_to = TEX__write__pixel(write_to, a);
            } else {
                write_to = TEX__write__pixel(write_to, b);
            }
        }
    }

    return;
}

// generate vertical stripes
void TEX__generate_face__vertical_stripes(TEX__faces faces, TEX__face_index face_index, TEX__pixel line_color_a, TEX__pixel line_color_b) {
    BASIC__address write_to;

    // get write to pointer
    write_to = TEX__calculate__faces_pointer(faces, face_index);

    // draw each row
    for (TEX__pixel_index row = 0; row < faces.height; row++) {
        // write each pixel in alternating order
        for (TEX__pixel_index pixel = 0; pixel < faces.height; pixel += 2) {
            // write first pixel
            write_to = TEX__write__pixel(write_to, line_color_a);

            // write second pixel
            write_to = TEX__write__pixel(write_to, line_color_b);
        }
    }

    return;
}

#endif
