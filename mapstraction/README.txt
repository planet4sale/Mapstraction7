$Id:

Description
===========
This module delivers support for the [Mapstraction javascript library][1], which
provides an abstraction layer for the various map providers including Google,
Yahoo!, and MapQuest. It allows you to quickly display maps on your site from
multiple providers and switch between providers without worrying about
differences in their APIs.

The module provides an API for rendering maps and a style plugin for the Views module. 
When the style is used, it will display nodes as points on a map. The latitude/longitude 
points can be provided by any view field including float fields or Location module
fields.

[1]: http://www.mapstraction.com/

Installation
============
1. Place in your modules directory
2. Download or clone Mapstraction v2 from GitHub, http://github.com/mapstraction/mxn/downloads 
   into a folder in your module directory called mapstraction. The resulting library path will be
   [modules directory]/mapstraction/mapstraction.
3. Enable the module at admin/build/modules
4. Configure a view with the proper style at admin/build/views or render a map directly in your
   code using mapstraction_render_map().

Configuration
=============
1. Configure a node type with Location or float fields to provide fields for latitude
   and longitude.
2. Create a new view and select Mapstraction as the style.
3. Add title, latitude, and longitude as view fields.
4. Configure the style by selecting your desired Mapping API, controls, and
   field mapping.