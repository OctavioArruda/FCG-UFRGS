//     Universidade Federal do Rio Grande do Sul
//             Instituto de Inform�tica
//       Departamento de Inform�tica Aplicada
//
//    INF01047 Fundamentos de Computa��o Gr�fica
//               Prof. Eduardo Gastal
//
//                   LABORAT�RIO 1
//

// Arquivos "headers" padr�es de C podem ser inclu�dos em um
// programa C++, sendo necess�rio somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Headers abaixo s�o espec�ficos de C++
#include <string>
#include <fstream>
#include <sstream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Cria��o de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Cria��o de janelas do sistema operacional

// Headers locais, definidos na pasta "include/"
#include "utils.h"

#define TOTAL_VERTICES_ONE 8
#define TOTAL_VERTICES_ZERO 32
#define CLOCK_POSITIONS 4

// Declara��o de v�rias fun��es utilizadas em main().  Essas est�o definidas
// logo ap�s a defini��o de main() neste arquivo.
GLuint BuildTriangles(float horizontal_shift, float vertical_shift); // Constr�i um tri�ngulo para renderiza��o
GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
void LoadShader(const char* filename, GLuint shader_id); // Fun��o utilizada pelas duas acima
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU

// Fun��es callback para comunica��o com o sistema operacional e intera��o do
// usu�rio. Veja mais coment�rios nas defini��es das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impress�o de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL vers�o 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto �, utilizaremos somente as
    // fun��es modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 500 colunas e 500 linhas
    // de pixels, e com t�tulo "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(500, 500, "INF01047 - 00246622 - Octavio Arruda", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos a fun��o de callback que ser� chamada sempre que o usu�rio
    // pressionar alguma tecla do teclado.
    glfwSetKeyCallback(window, KeyCallback);

    // Definimos a fun��o de callback que ser� chamada sempre que a janela for
    // redimensionada, por consequ�ncia alterando o tamanho do "framebuffer"
    // (regi�o de mem�ria onde s�o armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Indicamos que as chamadas OpenGL dever�o renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas fun��es definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Imprimimos no terminal informa��es sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de v�rtices e de fragmentos que ser�o utilizados
    // para renderiza��o. Veja slides 217-219 do documento "Aula_03_Rendering_Pipeline_Grafico.pdf".
    //
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" est�o fixados, sendo que assumimos a exist�ncia
    // da seguinte estrutura no sistema de arquivos:
    //
    //    + FCG_Lab_0X/
    //    |
    //    +--+ bin/
    //    |  |
    //    |  +--+ Release/  (ou Debug/ ou Linux/)
    //    |     |
    //    |     o-- main.exe
    //    |
    //    +--+ src/
    //       |
    //       o-- shader_vertex.glsl
    //       |
    //       o-- shader_fragment.glsl
    //       |
    //       o-- ...
    //
    GLuint vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    // Criamos um programa de GPU utilizando os shaders carregados acima
    GLuint program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    int index, stop, position;
    int secs, secs_aux;
    int secs_in_screen = 0;
    int binary_time[] = {0, 0, 0, 0};

    // VAO ids for each position in the clock
    GLuint vertex_array_objects_ids[4] = {0, 0, 0, 0};

    for (index = 0; index < CLOCK_POSITIONS; index++)
    {
        vertex_array_objects_ids[index] = BuildTriangles(index * .50 - 0.75, 0);
    }

    // Ficamos em um loop infinito, renderizando, at� que o usu�rio feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as opera��es de renderiza��o

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor �
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto �:
        // Vermelho, Verde, Azul, Alpha (valor de transpar�ncia).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Ilumina��o.
        //
        //           R     G     B     A
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima
        glClear(GL_COLOR_BUFFER_BIT);

        secs = (int)glfwGetTime() % 16;

        if (secs_in_screen != secs)
        {
            secs_in_screen = secs;
            secs_aux = secs;
            position = 3;

            for (index = 0; index < CLOCK_POSITIONS; index++)
            {
                binary_time[index] = 0;
            }

            stop = 0;

            do
            {
                if (secs_aux < 2)
                {
                    binary_time[position] = secs_aux;
                    stop = 1;
                }
                else
                {
                    binary_time[position] = secs_aux % 2;
                    secs_aux /= 2;
                    position--;
                }
            } while(!stop);

            printf("%d\n", secs);

            for (index = 0; index < CLOCK_POSITIONS; index++)
            {
                printf("%d", binary_time[index]);
                printf("\n");
            }
        }

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de v�rtice e fragmentos).
        glUseProgram(program_id);

        for (index = 0; index < CLOCK_POSITIONS; index++)
        {
            glBindVertexArray(vertex_array_objects_ids[index]);

            if (binary_time[index] == 0)
            {
                glDrawElements(GL_TRIANGLE_STRIP, TOTAL_VERTICES_ZERO + 2, GL_UNSIGNED_BYTE, 0);
            }
            else
            {
                glDrawElements(GL_TRIANGLE_STRIP, TOTAL_VERTICES_ONE - 1, GL_UNSIGNED_BYTE, (void*)(TOTAL_VERTICES_ZERO + 2 + 1));
            }

            // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
            // v�rtices apontados pelo VAO criado pela fun��o BuildTriangles(). Veja
            // coment�rios detalhados dentro da defini��o de BuildTriangles().
            glBindVertexArray(0);
        }

        // O framebuffer onde OpenGL executa as opera��es de renderiza��o n�o
        // � o mesmo que est� sendo mostrado para o usu�rio, caso contr�rio
        // seria poss�vel ver artefatos conhecidos como "screen tearing". A
        // chamada abaixo faz a troca dos buffers, mostrando para o usu�rio
        // tudo que foi renderizado pelas fun��es acima.
        // Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma intera��o do
        // usu�rio (teclado, mouse, ...). Caso positivo, as fun��es de callback
        // definidas anteriormente usando glfwSet*Callback() ser�o chamadas
        // pela biblioteca GLFW.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    return 0;
}

GLuint BuildTriangles(float horizontal_shift, float vertical_shift)
{
    int offset = TOTAL_VERTICES_ZERO;

    float theta = 2 * M_PI / 16;
    float angle;

    float external_x = .2;
    float internal_x = .1;

    float external_y = .4;
    float internal_y = .3;

    float digit_one_half_width = .05;

    GLfloat NDC_coefficients[TOTAL_VERTICES_ZERO * 4 + TOTAL_VERTICES_ONE * 4];

    int idx;

    for (idx = 0; idx < (TOTAL_VERTICES_ZERO + TOTAL_VERTICES_ONE); idx++)
    {
        NDC_coefficients[idx * 4 + 2] = 0.0f;
        NDC_coefficients[idx * 4 + 3] = 1.0f;
    }

    // X, Y coords. for every vertice of digit zero
    for (idx = 0; idx < 16; idx++)
    {
        angle = idx * theta;

        // External vertices for digit zero
        NDC_coefficients[idx * 8 + 0] = horizontal_shift + external_x * cos(angle);
        NDC_coefficients[idx * 8 + 1] = vertical_shift + external_y * sin(angle);

        // Internal vertices for digit zero
        NDC_coefficients[idx * 8 + 4] = horizontal_shift + internal_x * cos(angle);
        NDC_coefficients[idx * 8 + 5] = vertical_shift + internal_y * sin(angle);
    }

    // X, Y coords. for every vertice of digit one

    // Upper
    NDC_coefficients[(offset + 0) * 4] = -3 *  digit_one_half_width / 2 + horizontal_shift;
    NDC_coefficients[(offset + 0) * 4 + 1] = external_y + vertical_shift;

    NDC_coefficients[(offset + 1) * 4] = -3 * digit_one_half_width / 2 + horizontal_shift;
    NDC_coefficients[(offset + 1) * 4 + 1] = external_y - .2 + vertical_shift;

    NDC_coefficients[(offset + 2) * 4] = digit_one_half_width + horizontal_shift;
    NDC_coefficients[(offset + 2) * 4 + 1] = external_y + vertical_shift;

    NDC_coefficients[(offset + 3) * 4] = digit_one_half_width + horizontal_shift;
    NDC_coefficients[(offset + 3) * 4 + 1] = external_y - .2 + vertical_shift;

    // Lower
    NDC_coefficients[(offset + 4) * 4] = -digit_one_half_width + horizontal_shift;
    NDC_coefficients[(offset + 4) * 4 + 1] = external_y - .2 + vertical_shift;

    NDC_coefficients[(offset + 5) * 4] = digit_one_half_width + horizontal_shift;
    NDC_coefficients[(offset + 5) * 4 + 1] = external_y - .2 + vertical_shift;

    NDC_coefficients[(offset + 6) * 4] = -digit_one_half_width + horizontal_shift;
    NDC_coefficients[(offset + 6) * 4 + 1] = -external_y + vertical_shift;

    NDC_coefficients[(offset + 7) * 4] = digit_one_half_width + horizontal_shift;
    NDC_coefficients[(offset + 7) * 4 + 1] = -external_y + vertical_shift;

    GLuint VBO_NDC_coefficients_id;
    glGenBuffers(1, &VBO_NDC_coefficients_id);

    GLuint vertex_array_object_id;

    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_NDC_coefficients_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(NDC_coefficients), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(NDC_coefficients), NDC_coefficients);

    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(location);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLfloat color_coefficients[TOTAL_VERTICES_ZERO * 4 + TOTAL_VERTICES_ONE * 4];

    // Colors for zero - red
    for (idx = 0; idx < TOTAL_VERTICES_ZERO / 2; idx++)
    {
        color_coefficients[idx * 8 + 0] = 1.0f;
        color_coefficients[idx * 8 + 1] = 0.0f;
        color_coefficients[idx * 8 + 2] = 0.0f;
        color_coefficients[idx * 8 + 3] = 0.0f;

        color_coefficients[idx * 8 + 4] = 1.0f;
        color_coefficients[idx * 8 + 5] = 0.0f;
        color_coefficients[idx * 8 + 6] = 0.0f;
        color_coefficients[idx * 8 + 7] = 0.0f;
    }

    // Colors for one - blue
    for (idx = TOTAL_VERTICES_ZERO; idx < TOTAL_VERTICES_ZERO + 6; idx++)
    {
        color_coefficients[idx * 4 + 0] = 0.0f;
        color_coefficients[idx * 4 + 1] = 0.0f;
        color_coefficients[idx * 4 + 2] = 1.0f;
        color_coefficients[idx * 4 + 3] = 1.0f;
    }

    for (idx = TOTAL_VERTICES_ZERO + 6; idx < TOTAL_VERTICES_ZERO +8; idx++)
    {
        color_coefficients[idx * 4 + 0] = 0.0f;
        color_coefficients[idx * 4 + 1] = 0.0f;
        color_coefficients[idx * 4 + 2] = 1.0f;
        color_coefficients[idx * 4 + 3] = 1.0f;
    }

    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1; // "(location = 1)" em "shader_vertex.glsl"
    number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLubyte indices[TOTAL_VERTICES_ZERO + 2 + TOTAL_VERTICES_ONE];

    for (idx = 0; idx <= 33; idx++)
    {
        indices[idx] = idx % 32;
    }

    for (idx = TOTAL_VERTICES_ZERO + 2; idx < TOTAL_VERTICES_ZERO + TOTAL_VERTICES_ONE + 2; idx++)
    {
        indices[idx] = idx - 2;
    }

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    glBindVertexArray(0);

    return vertex_array_object_id;
}

// Carrega um Vertex Shader de um arquivo. Veja defini��o de LoadShader() abaixo.
GLuint LoadShader_Vertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // ser� aplicado nos v�rtices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

// Carrega um Fragment Shader de um arquivo. Veja defini��o de LoadShader() abaixo.
GLuint LoadShader_Fragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // ser� aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

// Fun��o auxilar, utilizada pelas duas fun��es acima. Carrega c�digo de GPU de
// um arquivo GLSL e faz sua compila��o.
void LoadShader(const char* filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela vari�vel "filename"
    // e colocamos seu conte�do em mem�ria, apontado pela vari�vel
    // "shader_string".
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    // Define o c�digo do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o c�digo do shader GLSL (em tempo de execu��o)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compila��o
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos mem�ria para guardar o log de compila��o.
    // A chamada "new" em C++ � equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compila��o
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ � equivalente ao "free()" do C
    delete [] log;
}

// Esta fun��o cria um programa de GPU, o qual cont�m obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Defini��o dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos mem�ria para guardar o log de compila��o.
        // A chamada "new" em C++ � equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "== End of link log\n";

        // A chamada "delete" em C++ � equivalente ao "free()" do C
        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Retorna o ID gerado acima
    return program_id;
}

// Defini��o da fun��o que ser� chamada sempre que a janela do sistema
// operacional for redimensionada, por consequ�ncia alterando o tamanho do
// "framebuffer" (regi�o de mem�ria onde s�o armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda regi�o do framebuffer. A
    // fun��o "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa � a opera��o de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula (slide 183 do documento "Aula_03_Rendering_Pipeline_Grafico.pdf").
    glViewport(0, 0, width, height);
}

// Defini��o da fun��o que ser� chamada sempre que o usu�rio pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // ==============
    // N�o modifique este loop! Ele � utilizando para corre��o automatizada dos
    // laborat�rios. Deve ser sempre o primeiro comando desta fun��o KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // ==============

    // Se o usu�rio pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Definimos o callback para impress�o de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// vim: set spell spelllang=pt_br :

