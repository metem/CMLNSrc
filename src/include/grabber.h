/*
 * CmlnGrabber.h
 *
 *  Created on: 11-02-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#ifndef CMLNGRABBER_H_
#define CMLNGRABBER_H_

#include <string.h>
#include <dc1394/dc1394.h>

namespace Grabber
{
typedef dc1394speed_t grabber_speed;
typedef dc1394framerate_t grabber_framerate;
typedef dc1394video_mode_t grabber_videomode;
typedef dc1394color_coding_t grabber_colorcoding;

class Cmln
{
private:
	dc1394_t* driver;
	dc1394camera_t* camera;
	dc1394video_frame_t* frame;
	uint32_t width;
	uint32_t height;
	grabber_colorcoding color_coding;
	grabber_videomode video_mode;

public:
	bool initialize();
	bool setIsoSpeed(grabber_speed iso_speed);
	bool setVideoMode(grabber_videomode video_mode);
	bool setFrameRate(grabber_framerate framerate);
	bool setup();
	bool setTransmision(bool on);
	bool close();

	bool getFrame(unsigned char* buffer);

	uint32_t getFrameBytes();
	uint32_t getFrameWidth()
	{
		return width;
	}
	uint32_t getFrameHeight()
	{
		return height;
	}
	grabber_colorcoding getColorCoding()
	{
		return color_coding;
	}
};

}
#endif /* CMLNGRABBER_H_ */
