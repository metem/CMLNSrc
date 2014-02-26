/*
 * plugin.cpp
 *
 *  Created on: 26-02-2014
 *      Author: Mateusz Ucher <mateusz.ucher@gmail.com>
 */

#include "cmlnsrc.h"

bool register_plugin(const Glib::RefPtr<Gst::Plugin>& plugin)
{
	if (!CMLNSrc::register_cmlnsrc(plugin))
		return false;

	/**
	 * More elements could be register here.
	 */

	return true;
}

static gboolean plugin_init(GstPlugin *plugin)
{
	Gst::init();
	return register_plugin(Glib::wrap(plugin, true));
}

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR, GST_VERSION_MINOR, cmln_lib, DESCRIPTION,
		plugin_init, VERSION, LICENSE, PACKAGE, URL)
