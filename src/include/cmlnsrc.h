/*
 * cmlnsrc.h
 *
 *  Created on: 26-02-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#ifndef CMLNSRC_H_
#define CMLNSRC_H_

#include <gstreamermm.h>
#include <gstreamermm/appsink.h>
#include <gstreamermm/private/pushsrc_p.h>

#include "config.h"
#include "grabber.h"

class CMLNSrc: public Gst::BaseSrc
{
private:
	Grabber::Cmln grabber;

public:
	static void base_init(Gst::ElementClass<CMLNSrc> *klass);
	static bool register_cmlnsrc(Glib::RefPtr<Gst::Plugin> plugin);

	Gst::FlowReturn create_vfunc(guint64 offset, guint size,
			Glib::RefPtr<Gst::Buffer>& buffer);
	bool negotiate_vfunc();
	bool start_vfunc();
	bool stop_vfunc();

	explicit CMLNSrc(GstBaseSrc *gobj);
};

#endif /* CMLNSRC_H_ */
