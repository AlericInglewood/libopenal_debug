#include <iostream>
#include <vector>
#include <AL/alc.h>
#include <AL/al.h>

extern "C" {
ALCdevice* alcCaptureOpenDevice(ALCchar const* deviceName, ALCuint freq, ALCenum fmt, ALCsizei bufsize);
}

ALenum last_error = AL_NO_ERROR;

char const* al_error()
{
  last_error = alGetError();
  switch(last_error)
  {
    case AL_NO_ERROR:
      return "AL_NO_ERROR";
    case AL_INVALID_NAME:
      return "AL_INVALID_NAME";
    case AL_INVALID_ENUM:
      return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE:
      return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION:
      return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY:
      return "AL_OUT_OF_MEMORY";
  }
  return "UNKNOWN ERROR CODE!";
}

int main()
{
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_FALSE)
  {
    std::cerr << "Enumeration Extension Not Found." << std::endl;
    return 2;
  }

  ALCchar const* output_device_names = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
  std::vector<ALCchar const*> output_device_name;
  for (ALCchar const* p = output_device_names; *p != '\0'; ++p)
  {
    output_device_name.push_back(p);
    while (*++p);
  }
  std::cout << "There are " << output_device_name.size() << " output devices:\n";
  for (std::vector<ALCchar const*>::iterator iter = output_device_name.begin(); iter != output_device_name.end(); ++iter)
    std::cout << *iter << '\n';
  ALCchar const* default_output_device_name = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
  std::cout << "The default output device is \"" << default_output_device_name << "\"." << std::endl;

  ALCchar const* capture_device_names = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
  std::vector<ALCchar const*> capture_device_name;
  for (ALCchar const* p = capture_device_names; *p != '\0'; ++p)
  {
    capture_device_name.push_back(p);
    while (*++p);
  }
  std::cout << "There are " << capture_device_name.size() << " input devices:\n";
  for (std::vector<ALCchar const*>::iterator iter = capture_device_name.begin(); iter != capture_device_name.end(); ++iter)
    std::cout << *iter << '\n';
  ALCchar const* default_capture_device_name = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
  std::cout << "The default input device is \"" << default_capture_device_name << "\"." << std::endl;

  ALCdevice* device;
  ALCcontext* context;

  // Initialize Open AL.
  device = alcOpenDevice(default_output_device_name);// Open default device.
  if (device != NULL) {
    context = alcCreateContext(device, NULL);	// Create context.
    if (context != NULL)
      alcMakeContextCurrent(context);		// Set active context.
  }
  alcDestroyContext(context);
  alcCloseDevice(device);

  device = alcOpenDevice(default_output_device_name);	// Open default device.
  if (device != NULL) {
    context = alcCreateContext(device, NULL);	// Create context.
    if (context != NULL)
      alcMakeContextCurrent(context);		// Set active context.
  }

  //ALCdevice* capture_device = alcCaptureOpenDevice(NULL, 44100, AL_FORMAT_STEREO16, 1024);
  ALCdevice* capture_device = alcCaptureOpenDevice(default_capture_device_name, 3200, AL_FORMAT_MONO16, 1280);
  if (alGetError() != AL_NO_ERROR)
    return 1;

  alcCaptureStart(capture_device);

  ALbyte buffer[22050];
  ALint sample;

  while(1)
  {
    alcGetIntegerv(capture_device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
    alcCaptureSamples(capture_device, (ALCvoid*)buffer, sample);

    // ... do something with the buffer 
  }

  alcCaptureStop(capture_device);
  alcCaptureCloseDevice(capture_device);

  alcDestroyContext(context);
  alcCloseDevice(device);

  return 0;
}
