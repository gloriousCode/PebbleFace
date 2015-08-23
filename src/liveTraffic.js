// Listen for when an AppMessage is received
/*Pebble.addEventListener('appmessage',
  function(e) {
    console.log('Travel AppMessage received!');
    getLiveTrafficReport();
  }                     
);

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getLiveTrafficReport() {
 var url = 'http://data.livetraffic.com/traffic/hazards/incident-open.json';

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      var dictionary = {};
      for(var i = 0; i< json.features; i++) {
        if(json.features[i].roads[0].suburb == "Gosford" || json.features[i].roads[0].suburb == "Woy Woy" || 
           json.features[i].roads[0].suburb == "Cowan" || json.features[i].roads[0].suburb == "Hawkesbury River" || 
           json.features[i].roads[0].suburb == "Mt White" || json.features[i].roads[0].suburb == "Calga" || 
           json.features[i].roads[0].suburb == "Ourimbah")
          {
            var trafficTitle = json.features[i].properties.headline;     
            var adviceA = json.features[i].properties.adviceA;
            var adviceB = json.features[i].properties.adviceB;
            var suburbDirection = json.features[i].roads[0].suburb + " " + json.features[i].roads[0].impactedLanes.affectedDirection;
            dictionary =
             {
              'KEY_TRAFFIC_TITLE': trafficTitle,
              'KEY_TRAFFIC_WARNING_ONE': adviceA,
              'KEY_TRAFFIC_WARNING_TWO': adviceB,
              'KEY_TRAFFIC_DIRECTION': suburbDirection
            };
            console.log('Traffic incident! ' + trafficTitle);
          }
      }
      if(dictionary != {}) {
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log('Traffic info sent to Pebble successfully!');
          },
          function(e) {
            console.log('Error sending Traffic info to Pebble!');
          }
        );
      }
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('Pebble livetraffic ready!');

    // Get the initial weather
    getLiveTrafficReport();
  }
);
*/
