/*
 * cmlnsrc.h
 *
 *  Created on: 26-02-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#ifndef CMLNSRC_H_
#define CMLNSRC_H_

#include <gstreamermm.h>
#include <gstreamermm/private/element_p.h>

#include "config.h"

class CMLNSrc: public Gst::Element
{
private:
	Glib::RefPtr<Gst::Pad> srcpad;

public:
	static void base_init(Gst::ElementClass<CMLNSrc> *klass);
	static bool register_cmlnsrc(Glib::RefPtr<Gst::Plugin> plugin);

	explicit CMLNSrc(GstElement *gobj);
};

#endif /* CMLNSRC_H_ */
