#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <AL/alc.h>
#include <AL/al.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <dlfcn.h>

#define DEFHOOK(function_name, return_type, param_list) \
  typedef return_type (*function_name##_type)param_list; \
  static function_name##_type function_name##_ptr

#define INITHOOK(function_name) \
  function_name##_ptr = (function_name##_type)dlsym(RTLD_NEXT, #function_name); \
  assert(function_name##_ptr != NULL)

//---------------------------------------------------------------------------------------------
// Function pointers to the real functions.

DEFHOOK(alcCaptureOpenDevice, ALCdevice*, (ALCchar const*, ALCuint, ALCenum, ALCsizei));
DEFHOOK(alcCaptureCloseDevice, ALCboolean, (ALCdevice*));
DEFHOOK(alcCaptureStart, void, (ALCdevice*));
DEFHOOK(alcCaptureStop, void, (ALCdevice*));
DEFHOOK(alcCaptureSamples, void, (ALCdevice*, ALCvoid*, ALCsizei));
DEFHOOK(alcOpenDevice, ALCdevice*, (ALCchar const*));
DEFHOOK(alcCloseDevice, ALCboolean, (ALCdevice*));
DEFHOOK(alcCreateContext, ALCcontext*, (ALCdevice*, ALCint const*));
DEFHOOK(alcDestroyContext, void, (ALCcontext*));
DEFHOOK(alcMakeContextCurrent, ALCboolean, (ALCcontext*));
DEFHOOK(alcGetString, ALCchar const*, (ALCdevice*, ALenum)); 

//---------------------------------------------------------------------------------------------
// Other global variables.

static ALCboolean initialized = ALC_FALSE;
static int debug_socket = 0;

void Dout(char const* format, ...)
{
  static char buf[256];
  static int print_timestamp = 0;

  int offset = 0;
  if (print_timestamp)
  {
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buf, 21, "%Y:%m:%d %H:%M:%S ", tm_info);
    offset += 21;
  }

  va_list ap;
  size_t len;
  va_start(ap, format);
  len = vsnprintf(buf + offset, sizeof(buf) - offset, format, ap);
  va_end(ap);
  if (len >= sizeof(buf) - offset)
    len = sizeof(buf) - offset - 1;
  write(debug_socket, buf, len + offset);

  print_timestamp = buf[len + offset - 1] == '\n';
}

void initialize()
{
  struct sockaddr_in serv_addr;
  if ((debug_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Error : Could not create debug socket.\n");
    return;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(10000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
  if (connect(debug_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\n Error : Connect failed.\n");
    return;
  }

  // First we run. Print an empty line.
  Dout("\n");

  INITHOOK(alcCaptureOpenDevice);
  INITHOOK(alcCaptureCloseDevice);
  INITHOOK(alcCaptureStart);
  INITHOOK(alcCaptureStop);
  INITHOOK(alcCaptureSamples);
  INITHOOK(alcOpenDevice);
  INITHOOK(alcCloseDevice);
  INITHOOK(alcCreateContext);
  INITHOOK(alcDestroyContext);
  INITHOOK(alcMakeContextCurrent);
  INITHOOK(alcGetString);
}

ALCdevice* alcCaptureOpenDevice(ALCchar const* deviceName, ALCuint freq, ALCenum fmt, ALCsizei bufsize)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcCaptureOpenDevice(\"%s\", %d, %u, %u) = ", deviceName, freq, fmt, bufsize);
  ALCdevice* ret = (*alcCaptureOpenDevice_ptr)(deviceName, freq, fmt, bufsize);
  Dout("%p\n", ret);

  return ret;
}

ALCboolean alcCaptureCloseDevice(ALCdevice* device)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcCaptureCloseDevice(%p) = ", device);
  ALCboolean ret = (*alcCaptureCloseDevice_ptr)(device);
  Dout("%d\n", ret);

  return ret;
}

void alcCaptureStart(ALCdevice* device)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcCaptureStart(%p)\n", device);
  (*alcCaptureStart_ptr)(device);
}

void alcCaptureStop(ALCdevice* device)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcCaptureStop(%p)\n", device);
  (*alcCaptureStop_ptr)(device);
}

void alcCaptureSamples(ALCdevice* device, ALCvoid* buf, ALCsizei samps)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  //Dout("alcCaptureSamples(%p, %p, %d)\n", device, buf, samps);
  (*alcCaptureSamples_ptr)(device, buf, samps);
}

ALCdevice* alcOpenDevice(ALCchar const* deviceSpecifier)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcOpenDevice(\"%s\") = ", deviceSpecifier);
  ALCdevice* ret = (*alcOpenDevice_ptr)(deviceSpecifier);
  Dout("%p\n", ret);

  return ret;
}

ALCboolean alcCloseDevice(ALCdevice* deviceHandle)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcCloseDevice(%p) = ", deviceHandle);
  ALCboolean ret = (*alcCloseDevice_ptr)(deviceHandle);
  Dout("%d\n", ret);

  return ret;
}

ALCcontext* alcCreateContext(ALCdevice* deviceHandle, ALCint const* attrList)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcCreateContext(%p, %p) = ", deviceHandle, attrList);
  ALCcontext* ret = (*alcCreateContext_ptr)(deviceHandle, attrList);
  Dout("%p\n", ret);

  return ret;
}

void alcDestroyContext(ALCcontext* context)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcDestroyContext(%p)\n", context);
  (*alcDestroyContext_ptr)(context);
}

ALCboolean alcMakeContextCurrent(ALCcontext* context)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcMakeContextCurrent(%p) = ", context);
  ALCboolean ret = (*alcMakeContextCurrent_ptr)(context);
  Dout("%d\n", ret);

  return ret;
}

ALCchar const* alcGetString(ALCdevice *device, ALenum param)
{
  if (!initialized)
  {
    initialize();
    initialized = ALC_TRUE;
  }

  Dout("alcGetString(%p, ", device);
  switch (param)
  {
    case ALC_DEFAULT_DEVICE_SPECIFIER:
      Dout("ALC_DEFAULT_DEVICE_SPECIFIER");
      break;
    case ALC_DEVICE_SPECIFIER:
      Dout("ALC_DEVICE_SPECIFIER");
      break;
    case ALC_EXTENSIONS:
      Dout("ALC_EXTENSIONS");
      break;
    case ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER:
      Dout("ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER");
      break;
    case ALC_CAPTURE_DEVICE_SPECIFIER:
      Dout("ALC_CAPTURE_DEVICE_SPECIFIER");
      break;
    case ALC_DEFAULT_ALL_DEVICES_SPECIFIER:
      Dout("ALC_DEFAULT_ALL_DEVICES_SPECIFIER");
      break;
    case ALC_ALL_DEVICES_SPECIFIER:
      Dout("ALC_ALL_DEVICES_SPECIFIER");
      break;
    default:
      Dout("%d", param);
      break;
  }
  Dout(") = \"");
  ALCchar const* ret = (*alcGetString_ptr)(device, param);
  if (device == NULL && (param == ALC_DEVICE_SPECIFIER || param == ALC_CAPTURE_DEVICE_SPECIFIER))
  {
    for (ALCchar const* p = ret; *p; ++p)
    {
      Dout("%s\\0", p);
      while (*++p);
    }
    Dout("\\0");
  }
  else
    Dout("%s", ret);
  Dout("\"\n");

  return ret;
}

