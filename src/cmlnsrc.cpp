/*
 * cmlnsrc.cpp
 *
 *  Created on: 26-02-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#include "cmlnsrc.h"

void CMLNSrc::base_init(Gst::ElementClass<CMLNSrc> *klass)
{
	klass->set_metadata(LNAME, KLASS, DESCRIPTION, AUTHOR);

	klass->add_pad_template(
			Gst::PadTemplate::create("src", Gst::PAD_SRC, Gst::PAD_ALWAYS,
					Gst::Caps::create_any()));
}

CMLNSrc::CMLNSrc(GstBaseSrc *gobj) :
		Gst::BaseSrc(gobj), vmode(*this, "vmode", 0), iso_speed(*this,
				"iso-speed", 400)
{
	if (!grabber.initialize())
		throw std::runtime_error("Failed to initialize video grabber");

	set_format(Gst::FORMAT_TIME);
	set_live(true);
}

bool CMLNSrc::set_caps_vfunc(const Glib::RefPtr<Gst::Caps>& caps)
{
	Gst::Fraction framerate;

	Gst::Structure allowed_caps = caps->get_structure(0);
	allowed_caps.get_field("framerate", framerate);
	switch (framerate.num)
	{
	case 30:
		if (!grabber.setFrameRate(
				Grabber::grabber_framerate::DC1394_FRAMERATE_30))
			throw std::runtime_error("Failed to set framerate");
		break;
	case 15:
		if (!grabber.setFrameRate(
				Grabber::grabber_framerate::DC1394_FRAMERATE_15))
			throw std::runtime_error("Failed to set framerate");
		break;
	default:
		return false;
	}

	return true;
}

Gst::FlowReturn CMLNSrc::create_vfunc(guint64 offset, guint size,
		Glib::RefPtr<Gst::Buffer>& buffer)
{
	buffer = Gst::Buffer::create(grabber.getFrameBytes());

	Glib::RefPtr<Gst::MapInfo> info(new Gst::MapInfo());
	buffer->map(info, Gst::MAP_WRITE);

	grabber.getFrame(info->get_data());

	buffer->unmap(info);

	return Gst::FLOW_OK;
}

bool CMLNSrc::negotiate_vfunc()
{
	return set_caps_vfunc(get_src_pad()->get_allowed_caps());
}

bool CMLNSrc::start_vfunc()
{
	bool result;
	switch (iso_speed)
	{
	case 100:
		result = grabber.setIsoSpeed(
				Grabber::grabber_speed::DC1394_ISO_SPEED_100);
		break;
	case 200:
		result = grabber.setIsoSpeed(
				Grabber::grabber_speed::DC1394_ISO_SPEED_200);
		break;
	case 400:
		result = grabber.setIsoSpeed(
				Grabber::grabber_speed::DC1394_ISO_SPEED_400);
		break;
	default:
		throw std::runtime_error("\nSupported ISO speeds:\n100\n200\n400");
	}

	if (!result)
		throw std::runtime_error("Failed to set ISO speed");

	switch (vmode)
	{
	case 0:
		result = grabber.setVideoMode(
				Grabber::grabber_videomode::DC1394_VIDEO_MODE_640x480_MONO8);
		break;
	case 1:
		result = grabber.setVideoMode(
				Grabber::grabber_videomode::DC1394_VIDEO_MODE_640x480_MONO16);
		break;
	case 2:
		result = grabber.setVideoMode(
				Grabber::grabber_videomode::DC1394_VIDEO_MODE_1280x960_MONO8);
		break;
	case 3:
		result = grabber.setVideoMode(
				Grabber::grabber_videomode::DC1394_VIDEO_MODE_1280x960_MONO16);
		break;
	case 4:
		result = grabber.setVideoMode(
				Grabber::grabber_videomode::DC1394_VIDEO_MODE_FORMAT7_0);
		break;
	case 5:
		result = grabber.setVideoMode(
				Grabber::grabber_videomode::DC1394_VIDEO_MODE_FORMAT7_1);
		break;
	case 6:
		result = grabber.setVideoMode(
				Grabber::grabber_videomode::DC1394_VIDEO_MODE_FORMAT7_2);
		break;
	default:
		throw std::runtime_error("\nSupported video modes:\n0 - 640x480_MONO8\n"
				"1 - 640x480_MONO16\n2 - 1280x960_MONO8\n3 - 1280x960_MONO16\n"
				"4 - FORMAT7_0\n5 - FORMAT7_1\n6 - FORMAT7_2");
	}

	if (!result)
		throw std::runtime_error("Failed to set video mode");

	if (!grabber.setup())
		throw std::runtime_error("Failed to setup the capture");

	set_blocksize(grabber.getFrameBytes());

	return grabber.setTransmision(true);
}

bool CMLNSrc::stop_vfunc()
{
	return grabber.close();
}

bool CMLNSrc::register_cmlnsrc(Glib::RefPtr<Gst::Plugin> plugin)
{
	Gst::ElementFactory::register_element(plugin, SNAME, RANK,
			Gst::register_mm_type<CMLNSrc>(LNAME));
	return true;
}
