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

CMLNSrc::CMLNSrc(GstElement *gobj) :
		Glib::ObjectBase(typeid(CMLNSrc)), Gst::Element(gobj)
{
	add_pad(srcpad = Gst::Pad::create(get_pad_template("src"), "src"));
}

bool CMLNSrc::register_cmlnsrc(Glib::RefPtr<Gst::Plugin> plugin)
{
	Gst::ElementFactory::register_element(plugin, SNAME, RANK,
			Gst::register_mm_type<CMLNSrc>(LNAME));
	return true;
}
