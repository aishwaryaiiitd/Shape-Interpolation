#include "utils.h"

extern bool controlPointsUpdated;
bool meshCreated = false;

std::vector<float> Contour1ptsCopy;
std::vector<float> Contour2ptsCopy;

void cleanup(GLFWwindow* window){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void clearLines(std::vector<float> &points, std::vector<float> &points2, std::vector<float> &Contour1pts, std::vector<float> &Contour2pts){
    points.clear();
    points2.clear();
    points.push_back(Contour1pts[0]);
    points.push_back(Contour1pts[1]);
    points.push_back(0.0);
    points2.push_back(Contour2pts[0]);
    points2.push_back(Contour2pts[1]);
    points2.push_back(0.0);
    controlPointsUpdated = true;
}

int findIndex(std::vector<float> &Contourpts, float x, float y){
    int sz = Contourpts.size();
    for (int i = 0; i< sz; i+=3){
        if(Contourpts[i]==x && Contourpts[i+1]==y){
            return i;
        }
    } 
    return 0;
}

void pushPoints(std::vector<float> &ContourptsCopy, std::vector<float> &Contourpts, int S, int E, int pointNo){
    if (pointNo<=0){
        for (int i = S+3; i<E; ++i)
            ContourptsCopy.push_back(Contourpts[i]);
    }
    else{
        int leap = (int)((E-S)/(3*pointNo));
        if(leap==0){
            std::vector<float> temp1;
            std::vector<float> temp2;
            for (int i = S+3; i<E; ++i)
                temp1.push_back(Contourpts[i]);
            while(pointNo!=0){
                for (int i =0; i <temp1.size()-3; i+=3){
                    temp2.push_back(temp1[i]);
                    temp2.push_back(temp1[i+1]);
                    temp2.push_back(temp1[i+2]);
                    if(pointNo!=0){
                        temp2.push_back((temp1[i]+temp1[i+3])/2.0);
                        temp2.push_back((temp1[i+1]+temp1[i+4])/2.0);
                        temp2.push_back((temp1[i+2]+temp1[i+5])/2.0);
                        pointNo--;
                    }
                }
                temp2.push_back(temp1[temp1.size()-3]);
                temp2.push_back(temp1[temp1.size()-2]);
                temp2.push_back(temp1[temp1.size()-1]);
                temp1.clear();
                for (int i =0; i <temp2.size(); i++)
                    temp1.push_back(temp2[i]);
                temp2.clear();
            }
            for (int i = 0; i<temp1.size(); ++i)
                ContourptsCopy.push_back(temp1[i]);
        }
        else{
        int k=0;
            for (int i = S+3; i<E; i+=3){
                ContourptsCopy.push_back(Contourpts[i]);
                ContourptsCopy.push_back(Contourpts[i+1]);
                ContourptsCopy.push_back(Contourpts[i+2]);
                if(k%leap==0 && pointNo>0){
                    ContourptsCopy.push_back((Contourpts[i]+Contourpts[i+3])/2.0);
                    ContourptsCopy.push_back((Contourpts[i+1]+Contourpts[i+4])/2.0);
                    ContourptsCopy.push_back((Contourpts[i+2]+Contourpts[i+5])/2.0);
                    pointNo--;
                }
                ++k;
            }
        }
    }
}

void topologicalMerge(std::vector<float> &mapContour1Points, std::vector<float> &mapContour2Points, std::vector<float> &Contour1pts, std::vector<float> &Contour2pts){

    int cont1sz = Contour1pts.size();
    int cont2sz = Contour2pts.size();
    mapContour1Points.push_back(Contour1pts[cont1sz-3]);
    mapContour1Points.push_back(Contour1pts[cont1sz-2]);
    mapContour1Points.push_back(Contour1pts[cont1sz-1]);
    mapContour2Points.push_back(Contour2pts[cont2sz-3]);
    mapContour2Points.push_back(Contour2pts[cont2sz-2]);
    mapContour2Points.push_back(Contour2pts[cont2sz-1]);
    
    int sz = mapContour1Points.size();
    
    for (int i=0; i<sz-3; i=i+3){
        int indS1 = findIndex(Contour1pts, mapContour1Points[i], mapContour1Points[i+1]);
        int indE1 = findIndex(Contour1pts, mapContour1Points[i+3], mapContour1Points[i+4]);
        int indS2 = findIndex(Contour2pts, mapContour2Points[i], mapContour2Points[i+1]);
        int indE2 = findIndex(Contour2pts, mapContour2Points[i+3], mapContour2Points[i+4]);

        std::cout<<indS1<<' '<<indE1<<' '<<indS2<<' '<<indE2<<endl;
        int dist1 = indE1-indS1;
        int dist2 = indE2-indS2;
        int pointsFor2 = (dist1-dist2)/3;
        int pointsFor1 = (dist2-dist1)/3;
        std::cout<<pointsFor1<<' '<<pointsFor2<<endl;
        Contour1ptsCopy.push_back(mapContour1Points[i]);
        Contour1ptsCopy.push_back(mapContour1Points[i+1]);
        Contour1ptsCopy.push_back(mapContour1Points[i+2]);
        pushPoints(Contour1ptsCopy, Contour1pts, indS1, indE1, pointsFor1);
        Contour2ptsCopy.push_back(mapContour2Points[i]);
        Contour2ptsCopy.push_back(mapContour2Points[i+1]);
        Contour2ptsCopy.push_back(mapContour2Points[i+2]);
        pushPoints(Contour2ptsCopy, Contour2pts, indS2, indE2, pointsFor2);
        std::cout<<"Length lists "<<Contour1ptsCopy.size()<<' '<<Contour2ptsCopy.size()<<endl;
    }
    int LL = Contour1ptsCopy.size();
    int BB = Contour2ptsCopy.size();
    std::cout<< LL<< ' ' << BB;
    
}

void compatibleRemeshing(std::vector<float> &Contour1pts, std::vector<float> &Contour2pts){
    
}

void printPoints(std::vector<float> &points){
    for(int i=0; i < points.size(); i++)
        std::cout << points.at(i) << ' ';
}

void addPoints(std::vector<float> &points, int x, int y, int w, int h, float pos){
    float rescaled_x = pos + ((0.6*x - 0) / (w - 0)) * (1.0 - (-1.0));
    float rescaled_y = -1*(-1.0 + ((1.0*(h - 0.6*y) - 0) / (h - 0)) * (1.0 - (-1.0)));
    points.push_back(rescaled_x);
    points.push_back(rescaled_y);
    points.push_back(0.0);
}


void showOptionsDialog(std::vector<float> &mapContour1Points, std::vector<float> &mapContour2Points, std::vector<float> &Contour1pts, std::vector<float> &Contour2pts,  ImGuiIO &io){
        ImGui::Begin("Window1");
  
        if(ImGui::Button("Clear")){
            // Clear points
            if(! meshCreated)
            clearLines(mapContour1Points, mapContour2Points,Contour1pts, Contour2pts);
        }

        if(ImGui::Button("Create Mesh")){
            // Clear points
            if(mapContour1Points.size()==mapContour2Points.size() && !meshCreated){
                std::cout<<"It happens"<<endl;
                std::cout<<mapContour1Points.size()<<endl;
                std::cout<<Contour1pts.size()<<endl;
                std::cout<<Contour2pts.size()<<endl;
                topologicalMerge(mapContour1Points, mapContour2Points, Contour1pts, Contour2pts);
                //compatibleRemeshing(Contour1pts, Contour2pts);
                meshCreated = true;
            }
            
        }
        
        ImGui::End();
}

void setVAO(unsigned int &VAO){
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

GLuint createShader(const char* filename, GLenum type)
{
    const GLchar* source = getShaderCode(filename);
    if (source == NULL) {
        fprintf(stderr, "Error opening %s: ", filename); perror("");
        return 0;
    }
    GLuint res = glCreateShader(type);
    glShaderSource(res, 1, &source, NULL);
    free((void*)source);

    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        // fprintf(stderr, "%s:", filename);
        std::cout << "Error in compilation of :" << filename << std::endl;
        glDeleteShader(res);
        return 0;
    }

    return res;
}

const char * setGLSLVersion(){
    #if __APPLE__
    // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif

    return glsl_version;
}

int openGLInit(){
    #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        exit(1);
    }

    //Enable multisampling
    glEnable(GL_MULTISAMPLE);

    // Enable Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set point size
    glPointSize(10.0f);

    // Enable smooth point rendering
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

unsigned int createProgram(const char *vshader_filename, const char* fshader_filename)
{
    //Create shader objects
    GLuint vs, fs;
    if ((vs = createShader(vshader_filename, GL_VERTEX_SHADER))   == 0) return 0;
    if ((fs = createShader(fshader_filename, GL_FRAGMENT_SHADER)) == 0) return 0;

    //Creare program object and link shader objects
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    GLint link_ok;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        // fprintf(stderr, "glLinkProgram error:");
        // printLog(program);
        std::cout << "Linking error " << std::endl  ;
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}


char * getShaderCode(const char* filename)
{
    FILE* input = fopen(filename, "rb");
    if(input == NULL) return NULL;

    if(fseek(input, 0, SEEK_END) == -1) return NULL;
    long size = ftell(input);
    if(size == -1) return NULL;
    if(fseek(input, 0, SEEK_SET) == -1) return NULL;

    /*if using c-compiler: dont cast malloc's return value*/
    char *content = (char*) malloc( (size_t) size +1  );
    if(content == NULL) return NULL;

    fread(content, 1, (size_t)size, input);
    if(ferror(input)) {
        free(content);
        return NULL;
    }

    fclose(input);
    content[size] = '\0';
    return content;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* setupWindow(int width, int height)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(0);

    // Decide GL+GLSL versions
    const char * glsl_version = setGLSLVersion();
    

    // Create window with graphics context
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, "Window Title", NULL, NULL);
    if (window == NULL)
        exit(0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    int status = openGLInit();
    if(!status){
        std::cout << "Initialized OpenGL succesfully " << std::endl;
    }
    std::cout<< "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

