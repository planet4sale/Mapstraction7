document.namespaces;

(function($) {

Drupal.behaviors.mapstraction = {
  'attach': function(context, settings) {	
  Drupal.mapstraction = [];
	  
	  
	// return a MXN lat/long point from a row
  function preparePoint(row) {
    return new mxn.LatLonPoint(Number(row.latitude), Number(row.longitude));
  }

  // Initialize marker object from a point data
  function prepareMarker(row, hover) {
    var markerPoint = preparePoint(row);
    var marker = new mxn.Marker(markerPoint);
      
      // set info bubble if we have a title
    if (row.text) {
      marker.setInfoBubble('<div class="mapstraction-info-window">'+ row.text +'</div>');
    }
      
      // override default icon if provided
    if (row.icon) {
      marker.setIcon(row.icon.url, [row.icon.width, row.icon.height], [row.icon.width/2,row.icon.height]);
    }

      // set all attributes
    if (row.attributes){
      $.each(row.attributes, marker.setAttribute);
    }        

    marker.setHover(hover);
    
    return marker;
  }

	// generate a polyline to add to a map
  function preparePolyline(row, points) {
    var poly = new mxn.Polyline(points);

		//Shuffle colors
		var color = '#';
		while (color.length < 7) {
			color = color.concat((Math.round(Math.random() * 15)).toString(16));
		}

		poly.color = color;
		poly.width = 2;
		poly.opacity = 1;
		poly.closed = false;
		return poly;
  }

	// add markers and/or polylines to the map
  function getGroupRenderer(map, behaviours, overlays) {
    return function(row) {
      var points = $.map(row.markers, overlays.markers ? 
        function(point) {
  	      var marker = prepareMarker(point, behaviours.hover);
  	      map.addMarker(marker);
  	      return marker.location;
        } : preparePoint
      );

      if (overlays.polylines) {
        var poly = preparePolyline(row, points);
        map.addPolyline(poly);
      }
    };
  }	  
 
    if (typeof(settings.mapstraction) === 'object'){
	 // Iterate over each map added to the page
	    $(settings.mapstraction).each(function() {

var map_id = $(this).attr('mapId');
		    var map = Drupal.mapstraction[settings.mapstraction.mapId] = new mxn.Mapstraction(map_id,'openlayers');
		    
    // Set up markers and info bubbles.
    $.map(this.markers, getGroupRenderer(map, this.behaviours, this.overlays));		    

     // Set up controls.
     map.addControls(this.controls);
    
     // add terrain option if dealing with Google
     if (this.controls.map_type && typeof G_PHYSICAL_MAP != 'undefined') {
       map.getMap().addMapType(G_PHYSICAL_MAP);
     }

     // default map type
     if (this.controls.default_maptype) {
       var maptype = this.controls.default_maptype;
       if(typeof(maptype) == 'string'){
         maptype = eval(maptype);
       }
       try {//OpenLayers throw 'Not implemented'
         map.setMapType(maptype);
       } catch(err) {}
     }


    // declutter markers
    if (this.behaviours && this.behaviours.declutter) {
      map.declutterMarkers();
    }     

     // Set auto center and zoom.
     if (this.initialPoint.auto) {
       map.autoCenterAndZoom();
     }
     else {
       var startPoint = new mxn.LatLonPoint(Number(this.initialPoint.latitude), Number(this.initialPoint.longitude));
       map.setCenterAndZoom(startPoint, Number(this.initialPoint.zoom));
     }
     
  });
  }
 }  
};

}(jQuery));