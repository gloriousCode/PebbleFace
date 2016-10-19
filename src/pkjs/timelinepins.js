Pebble.addEventListener('ready', function() {
  console.log('Timeline pin JS loaded');
  addGymPin();
  addMeditationPin(); 
});

function addMeditationPin() {
    var date = new Date();
  if(date.getDay() === 0 || date.getDay() === 6) {
    return;
  }
  date.setMinutes(30);
  date.setHours(20);
  
  // Create the pin
  var pin = {
    "id": "pin-meditation-" + date.getDate()  + date.getDay(),
    "time": date.toISOString(),
    "layout": {
      "type": "genericPin",
      "title": "Meditation starts now",
      "tinyIcon": "system://images/DAY_SEPARATOR",
      "primaryColor": "#FFFFFF",
      "secondaryColor": "#666666",
      "backgroundColor": "#222222"
    }
  };
  
   console.log('Inserting pin in the future: ' + JSON.stringify(pin));
  deleteUserPin(pin, function(responseText) { 
    console.log('Result: ' + responseText);
  });
  insertUserPin(pin, function(responseText) { 
    console.log('Result: ' + responseText);
  });
}

function addGymPin() {
    var date = new Date();
  if(date.getDay() !== 0 && date.getDay() !== 1 && date.getDay() !==2 && date.getDay() !== 4 && date.getDay() !== 5) {
    return;
  }
  date.setMinutes(0);
  if(date.getDay() === 0) {
      date.setHours(11);
  }
 if(date.getDay() == 1 || date.getDay() ==   2 || date.getDay() ==   4 || date.getDay() == 5)
   {
     date.setHours(17);
   }

  // Create the pin
  var pin = {
    "id": "pin-gym-" + date.getDate()  + date.getDay(),
    "time": date.toISOString(),
    "layout": {
      "type": "genericPin",
      "title": "Go to gym",
      "tinyIcon": "system://images/MUSIC_EVENT",
      "primaryColor": "#FFFFFF",
      "secondaryColor": "#666666",
      "backgroundColor": "#222222"
    }
  };
  
   console.log('Inserting pin in the future: ' + JSON.stringify(pin));
  deleteUserPin(pin, function(responseText) { 
    console.log('Result: ' + responseText);
  });
  insertUserPin(pin, function(responseText) { 
    console.log('Result: ' + responseText);
  });
}

/******************************* timeline lib *********************************/

// The timeline public URL root
var API_URL_ROOT = 'https://timeline-api.getpebble.com/';

/**
 * Send a request to the Pebble public web timeline API.
 * @param pin The JSON pin to insert. Must contain 'id' field.
 * @param type The type of request, either PUT or DELETE.
 * @param callback The callback to receive the responseText after the request has completed.
 */
function timelineRequest(pin, type, callback) {
  // User or shared?
  var url = API_URL_ROOT + 'v1/user/pins/' + pin.id;

  // Create XHR
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    console.log('timeline: response received: ' + this.responseText);
    callback(this.responseText);
  };
  xhr.open(type, url);

  // Get token
  Pebble.getTimelineToken(function(token) {
    // Add headers
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('X-User-Token', '' + token);

    // Send
    xhr.send(JSON.stringify(pin));
    console.log('timeline: request sent.');
  }, function(error) { console.log('timeline: error getting timeline token: ' + error); });
}

/**
 * Insert a pin into the timeline for this user.
 * @param pin The JSON pin to insert.
 * @param callback The callback to receive the responseText after the request has completed.
 */
function insertUserPin(pin, callback) {
  timelineRequest(pin, 'PUT', callback);
}

/**
 * Delete a pin from the timeline for this user.
 * @param pin The JSON pin to delete.
 * @param callback The callback to receive the responseText after the request has completed.
 */
function deleteUserPin(pin, callback) {
  timelineRequest(pin, 'DELETE', callback);
}

/***************************** end timeline lib *******************************/