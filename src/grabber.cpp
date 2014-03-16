/*
 * CmlnGrabber.cpp
 *
 *  Created on: 11-02-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#include "grabber.h"

using namespace Grabber;

bool Cmln::initialize()
{
	driver = dc1394_new();

	if (driver)
	{
		dc1394camera_list_t* cameraList;
		dc1394error_t error = dc1394_camera_enumerate(driver, &cameraList);
		if ((error == DC1394_SUCCESS) && (cameraList->num > 0))
		{
			camera = dc1394_camera_new(driver, cameraList->ids[0].guid);
			if (camera)
			{
				dc1394_camera_free_list(cameraList);
				return true;
			}
		}
	}
	return false;
}

bool Cmln::setIsoSpeed(grabber_speed iso_speed)
{
	dc1394error_t error = dc1394_video_set_iso_speed(camera, iso_speed);

	return (error == DC1394_SUCCESS);
}

bool Cmln::setVideoMode(grabber_videomode video_mode)
{
	if (dc1394_video_set_mode(camera, video_mode) == DC1394_SUCCESS)
	{
		this->video_mode = video_mode;
		if (dc1394_get_image_size_from_video_mode(camera, video_mode, &width,
				&height) == DC1394_SUCCESS)
		{
			if (video_mode == grabber_videomode::DC1394_VIDEO_MODE_FORMAT7_0)
			{
				color_coding = dc1394color_coding_t::DC1394_COLOR_CODING_RGB8;
				return true;
			}

			if (dc1394_get_color_coding_from_video_mode(camera, video_mode,
					&color_coding) == DC1394_SUCCESS)
				return true;
		}
	}
	return false;
}

bool Cmln::setFrameRate(grabber_framerate framerate)
{
	dc1394error_t error = dc1394_video_set_framerate(camera, framerate);
	return (error == DC1394_SUCCESS);
}

bool Cmln::setup()
{
	dc1394error_t error = dc1394_capture_setup(camera, 4,
	DC1394_CAPTURE_FLAGS_DEFAULT);
	return (error == DC1394_SUCCESS);
}

bool Cmln::setTransmision(bool on)
{
	dc1394error_t error;
	if (on)
		error = dc1394_video_set_transmission(camera, DC1394_ON);
	else
		error = dc1394_video_set_transmission(camera, DC1394_OFF);

	return (error == DC1394_SUCCESS);
}

bool Cmln::close()
{
	if (setTransmision(false))
	{
		dc1394_capture_stop(camera);
		dc1394_camera_free(camera);
		dc1394_free(driver);
		return true;
	}
	return false;
}

bool Cmln::getFrame(unsigned char* buffer)
{
	dc1394error_t error = dc1394_capture_dequeue(camera,
			DC1394_CAPTURE_POLICY_WAIT, &frame);
	if ((error == DC1394_SUCCESS) && frame)
	{
		if (video_mode == grabber_videomode::DC1394_VIDEO_MODE_FORMAT7_0)
		{
			dc1394_bayer_decoding_8bit(frame->image, buffer, frame->size[0],
					frame->size[1],
					dc1394color_filter_t::DC1394_COLOR_FILTER_GBRG,
					dc1394bayer_method_t::DC1394_BAYER_METHOD_NEAREST);
		}
		dc1394_capture_enqueue(camera, frame);
		return true;
	}
	return false;
}

uint32_t Cmln::getFrameBytes()
{
	switch (color_coding)
	{
	case grabber_colorcoding::DC1394_COLOR_CODING_RGB8:
		return getFrameWidth() * getFrameHeight() * 3;
	default:
		return 0;
	}
}
