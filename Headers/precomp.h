#pragma once

#include <stdio.h>
#include <assert.h> 
#include <string>
#include <math.h>
#include <sys/time.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES3/gl3.h>
#include <vector>

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#ifdef RASPBERRY
#include "bcm_host.h"
#endif

//#include <stb_image.h>

#include <X11/Xlib.h>
#include<X11/extensions/Xrandr.h>
#include <X11/Xutil.h>


// Include GLM
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Input.h"
#include "Graphics.h"