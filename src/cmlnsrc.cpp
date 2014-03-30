/*
 * cmlnsrc.cpp
 *
 *  Created on: 26-02-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#include "cmlnsrc.h"

void CMLNSrc::base_init(Gst::ElementClass<CMLNSrc> *klass)
{
	//TODO add gstreamer properties

	Glib::RefPtr<Gst::Caps> caps = Gst::Caps::create_simple("video/x-raw");
	caps->set_simple("format", "RGB");
	caps->set_simple("width", 1280);
	caps->set_simple("height", 960);
	caps->set_simple("bpp", 8);
	caps->set_simple("depth", 8);
	caps->set_simple("framerate", Gst::Fraction(30, 1));

	klass->set_metadata(LNAME, KLASS, DESCRIPTION, AUTHOR);

	klass->add_pad_template(
			Gst::PadTemplate::create("src", Gst::PAD_SRC, Gst::PAD_ALWAYS,
					caps));
}

CMLNSrc::CMLNSrc(GstBaseSrc *gobj) :
		Gst::BaseSrc(gobj)
{
	set_format(Gst::FORMAT_TIME);
	set_live(true);

	//TODO change to gstreamer debug/error messages

	if (!grabber.initialize())
		throw std::runtime_error("Failed to initialize video grabber");

	if (!grabber.setIsoSpeed(Grabber::grabber_speed::DC1394_ISO_SPEED_400))
		throw std::runtime_error("Failed to set ISO speed");

	if (!grabber.setVideoMode(
			Grabber::grabber_videomode::DC1394_VIDEO_MODE_FORMAT7_0))
		throw std::runtime_error("Failed to set video mode");

	if (!grabber.setFrameRate(Grabber::grabber_framerate::DC1394_FRAMERATE_30))
		throw std::runtime_error("Failed to set framerate");

	if (!grabber.setup())
		throw std::runtime_error("Failed to setup the capture");

	set_blocksize(grabber.getFrameBytes());
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
	return true;
}

bool CMLNSrc::start_vfunc()
{
	grabber.setTransmision(true);
	return true;
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
