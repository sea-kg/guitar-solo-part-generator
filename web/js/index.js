// https://github.com/stuartmemo/qwerty-hancock/blob/master/dist/qwerty-hancock.js

function parsePageParams() {
	var loc = location.search.slice(1);
	var arr = loc.split("&");
	var result = {};
	var regex = new RegExp("(.*)=([^&#]*)");
	for(var i = 0; i < arr.length; i++){
		if(arr[i].trim() != ""){
			var p = regex.exec(arr[i].trim());
			console.log("results: " + JSON.stringify(p));
			if(p == null)
				result[decodeURIComponent(arr[i].trim().replace(/\+/g, " "))] = '';
			else
				result[decodeURIComponent(p[1].replace(/\+/g, " "))] = decodeURIComponent(p[2].replace(/\+/g, " "));
		}
	}
	console.log(JSON.stringify(result));
	return result;
}

window.pageParams = parsePageParams();


function containsPageParam(name){
	return (typeof pageParams[name] !== "undefined");
}


function changeLocationState(newPageParams) {
	var url = '';
	var params = [];
	// console.log("changeLocationState");
	console.log("changeLocationState, newPageParams = ", newPageParams);
	for(var p in newPageParams){
		params.push(encodeURIComponent(p) + "=" + encodeURIComponent(newPageParams[p]));
	}
	// console.log("changeLocationState", params);
	// console.log("changeLocationState", window.location.pathname + '?' + params.join("&"));
	window.history.pushState(newPageParams, document.title, window.location.pathname + '?' + params.join("&"));
	window.pageParams = parsePageParams();
}
changeLocationState(window.pageParams)

window.addEventListener('popstate', function(event) {
    // The popstate event is fired each time when the current history entry changes.

    // history.back();
    console.log("popstate");
    initFilters();
    window.pageParams = parsePageParams();
    console.log(window.pageParams);

    // var r = confirm("You pressed a Back button! Are you sure?!");

    // if (r == true) {
        // Call Back button programmatically as per user confirmation.
    //    history.back();
        // Uncomment below line to redirect to the previous page instead.
        // window.location = document.referrer // Note: IE11 is not supporting this.
    // } else {
        // Stay on the current page.
    //    history.pushState(null, null, window.location.pathname);
    // }

    // history.pushState(null, null, window.location.pathname);

}, false);

window.onhashchange = function() {
    console.log("onhashchange");
}



var notes = ['A', 'A#', 'B', 'C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#'];

// TODO move to backend side
var getFrequencyOfNote = function (note) {
    var notes = ['A', 'A#', 'B', 'C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#'],
        key_number,
        octave;

    if (note.length === 3) {
        octave = note.charAt(2);
    } else {
        octave = note.charAt(1);
    }

    key_number = notes.indexOf(note.slice(0, -1));

    if (key_number < 3) {
        key_number = key_number + 12 + ((octave - 1) * 12) + 1;
    } else {
        key_number = key_number + ((octave - 1) * 12) + 1;
    }

    return 440 * Math.pow(2, (key_number - 49) / 12);
};
var nodes = [];

window.AudioContext = window.AudioContext || window.webkitAudioContext;
window.ac = new AudioContext()
window.waves = createWaves(ac);

// The setup includes tail part, which is common for all nodes
// And head - set of nodes added for each note
// It looks like this:
//
// ------------- HEAD ---------------- | --------- TAIL --------------------
//                                     |
// [ |Oscillator|->|Biquad|->|Gain|-> ][ |Gain|->|Dynamics  |->|Destination| ]
//   |Periodic  |  |Filter|  |ADSR|              |Compressor|
//   |Wave      |
//
// The first oscillator can be doubled by another one to play
// at note frequency + timbre detune (if detune is set for the timbre)
//
// So we crate an oscillator (or two) + filter + ADSR gain to play each note.
// This way we create a lot of audio nodes.
// Musicial.js handles this by creating a queue of notes and passed only a limited set of notes to web audio API.
//

function updateTabulaturNoteEnded(idx) {
    window.tabeditor.render(idx + 1);
}

function addNote(_note, _time, _duration, _cleanuptime, idx) {
    console.log(_note)
    var note = {};
    note["time"] = _time;
    note["frequency"] = getFrequencyOfNote(_note.note);
    note["duration"] = _duration;
    note["velocity"] = 0.4;
    note["cleanuptime"] = _cleanuptime;

    var timbre = wave.defs;
    var startTime = note.time;
    var releaseTime = startTime + note.duration;
    var attackTime = Math.min(releaseTime, startTime + timbre.attack);
    var decayTime = timbre.decay *
          Math.pow(440 / note.frequency, timbre.decayfollow);
    var decayStartTime = attackTime;
    var stopTime = releaseTime + timbre.release;
    var doubled = timbre.detune && timbre.detune != 1.0;
    var amp = timbre.gain * note.velocity * (doubled ? 0.5 : 1.0);

    var masterGain = ac.createGain();
    masterGain.connect(out);
    masterGain.gain.setValueAtTime(0, startTime);
    // ATTACK
    masterGain.gain.linearRampToValueAtTime(amp, attackTime);
    // DECAY
    //   For the beginning of the decay, use linearRampToValue instead
    //   of setTargetAtTime, because it avoids http://crbug.com/254942.
    while (decayStartTime < attackTime + 1/32 &&
           decayStartTime + 1/256 < releaseTime) {
      // Just trace out the curve in increments of 1/256 sec
      // for up to 1/32 seconds.
      decayStartTime += 1/256;
      masterGain.gain.linearRampToValueAtTime(
          amp * (timbre.sustain + (1 - timbre.sustain) *
              Math.exp((attackTime - decayStartTime) / decayTime)),
          decayStartTime);
    }
    // SUSTAIN
    //   For the rest of the decay, use setTargetAtTime.
    masterGain.gain.setTargetAtTime(amp * timbre.sustain,
        decayStartTime, decayTime);
    // RELEASE
    //   Then at release time, mark the value and ramp to zero.
    masterGain.gain.setValueAtTime(amp * (timbre.sustain + (1 - timbre.sustain) *
        Math.exp((attackTime - releaseTime) / decayTime)), releaseTime);
    masterGain.gain.linearRampToValueAtTime(0, stopTime);

    //var f = g;
    var filter = ac.createBiquadFilter();
    filter.frequency.value =
        wave.defs.cutoff + note.frequency * wave.defs.cutfollow;
        filter.Q.value = wave.defs.resonance;
        filter.connect(masterGain);
    nodes.push(filter)
    nodes.push(masterGain)

    var osc = ac.createOscillator();
    // Configure periodic wave
    osc.frequency.value = note.frequency;
    var pwave = wave.wave;
    // Check the note frequency and if it is greater than wave frequency
    // then switch to another wave for higher frequency
    // Waves for different frequences are pre-created in createWaves()
    if (wave.freq) {
      var bwf = 0;
      // Look for a higher-frequency variant.
      for (var k in wave.freq) {
        wf = Number(k);
        if (note.frequency > wf && wf > bwf) {
          bwf = wf;
          pwave = wave.freq[bwf];
        }
      }
    }
    osc.setPeriodicWave(pwave);
    osc.connect(filter);
    osc.start(startTime);
    console.log("start ", startTime);
    osc.stop(stopTime);
    osc.onended = function() { 
        // console.log(_note);
        updateTabulaturNoteEnded(idx);
        // $('#currentnote').html(_note.note);
    };
    nodes.push(osc);

    if (doubled) {
        var o2 = ac.createOscillator();
        o2.frequency.value = note.frequency * timbre.detune;
        o2.setPeriodicWave(wave.wave);
        o2.connect(filter);
        o2.start(startTime);
        o2.stop(stopTime);
        o2.onended = function() { 
            
            // console.log("o2", _note, idx);
            // $('#currentnote').html(_note.note);
        };
        nodes.push(o2);
    }
    startTime += note.duration*1;
}

function stop() {
    for (var i = 0; i < nodes.length; i++) {
        nodes[i].disconnect();
    }
    nodes = []
    window.ac.close();
    document.getElementById('gspg_player_play').style.display = '';
    document.getElementById('gspg_player_stop').style.display = 'none';
}

function toTime(timeStr, timeOneTick) {
    var r = timeStr.split("/");
    return timeOneTick * parseInt(r[0], 10) / parseInt(r[1], 10);
}

function play() {
    stop();
    document.getElementById('gspg_player_play').style.display = 'none';
    document.getElementById('gspg_player_stop').style.display = '';

    updateTabulaturNoteEnded(-1);

    window.ac = new AudioContext()
    window.wave = waves['piano'];
    var dcn = ac.createDynamicsCompressor();
    dcn.ratio = 16;
    dcn.attack = 0.0005;
    dcn.connect(ac.destination);
    window.out = ac.createGain();
    window.out.connect(dcn);
    var _timeOneTick = 1.2; // speed 
    var _timeStart = 0;
    window.wave = waves['guitar'];
    for (var i = 0; i < window.tabeditor.data.length; i++) {
        var n = window.tabeditor.data[i];
        _timeStart = toTime(n.time, _timeOneTick);
        var _duration = toTime(n.duration, _timeOneTick) - 0.05;
        var idx = i;
        addNote(window.tabeditor.data[i], _timeStart, _duration, _timeStart, idx);
    }
}

// window.guitarTunings = ["E4", "B3", "G3", "D3", "A2", "E2"]

function generate() {
    stop();
    var filters = {}
    for (var i = 0; i < window.available_filters.length; i++) {
        var f = window.available_filters[i]
        var filter_name = f['name'];
        if (containsPageParam(filter_name)) {
            filters[filter_name] = pageParams[filter_name];
        }
    }

    $.ajax({
        url: "./api/v1/solo-generate",
        method: "GET",
        data: filters
    }).done(function(resp){
        console.log(JSON.stringify(resp));
        $('#tabulatur').html(resp['tabulatur']);
        tabeditor.updateData(resp["part"]);
        console.log(tabeditor.sirealizeToString());
        tabeditor.render();
    }).fail(function(err){
        console.error(err)
        tabeditor.data = [];
        tabeditor.render();
        $('#tabulatur').html(err.statusText);
    })
}

function changedValueOfList(e) {
    var name = e.attributes['filter-name'].nodeValue
    pageParams[name] = e.value
    changeLocationState(pageParams)
}

function appendFilterSelectList(_filter) {
    var name = _filter["name"];
    var pre_val = "";
    if (containsPageParam(name)) {
        pre_val = pageParams[name]
    } else {
        pre_val = _filter["values"][0]["value"]
    }
    var _content = ''
        + '<div class="gspg-filter-row">'
        + '  <div class="gspg-filter-row-title">'
        + '    ' + _filter["caption"]
        + '  </div>'
        + '  <select class="gspg-select-control" filter-name="' + name + '" id="filter_' + name + '" onchange="changedValueOfList(this)">'
    for (var i in _filter["values"]) {
        var _value = _filter["values"][i]["value"]
        var _caption = _filter["values"][i]["caption"]
        _content += '<option value="' + _value + '"' + (pre_val == _value ? 'selected' : '') + '>' + _caption + '</option>';
    }
    _content += ''
        + '  </select>'
        + '</div>';

        // onchange="myFunction()"
    document.getElementById('filters').innerHTML += _content;
}

function changedValueOfCheckboxList(e) {
    var filter_name = e.getAttribute('filter-name')
    var newValue = !e.classList.contains("checked");
    if (newValue) {
        e.classList.add("checked");
        e.getElementsByClassName('gspg-checkbox-check')[0].classList.add("checked");
    } else {
        e.classList.remove("checked");
        e.getElementsByClassName('gspg-checkbox-check')[0].classList.remove("checked");
    }

    var fields = document.getElementsByClassName('gspg-checkbox-container');
    var values = []
    for (var i = 0; i < fields.length; i ++) {
        var field = fields[i]
        if (field.getAttribute('filter-name') == filter_name) {
            if (field.classList.contains("checked")) {
                values.push(field.getAttribute('filter-value'))
            }
        }
    }
    pageParams[filter_name] = values.join("|")
    changeLocationState(pageParams)
}

function appendFilterCheckboxList(_filter) {
    var name = _filter["name"];
    var values = _filter["values"]
    var checked_vals = [];
    if (containsPageParam(name)) {
        checked_vals = pageParams[name]
        checked_vals = checked_vals.split("|")
    } else {
        for (var i in values) {
            if (values[i]["checked"] == true) {
                checked_vals.push(values[i]["value"])
            }
        }
    }
    console.log(checked_vals)

    var _content = ''
        + '<div class="gspg-filter-row">'
        + '   <div class="gspg-filter-row-title">' + _filter['caption'] + '</div>';
    for (var i in values) {
        var _value = values[i]["value"]
        var _caption = values[i]["caption"]
        var _checked = values[i]["checked"] == true ? "checked" : "";
        if (checked_vals.indexOf(_value) !== -1) {
            _checked = 'checked'
        } else {
            _checked = ''
        }
        _content += ''
            + '<div class="gspg-checkbox-container ' + _checked + '" '
            + ' filter-name="' + name + '" '
            + ' filter-value="' + _value + '" '
            + ' onclick="changedValueOfCheckboxList(this)"'
            + '><div class="gspg-checkbox-check ' + _checked + '"></div><div class="gspg-checkbox-caption">' + _caption + '</div></div>';
    }
    _content += '</div>';
    document.getElementById('filters').innerHTML += _content;

}

function applyAllowedFilters(resp) {
    for (var t in resp["result"]) {
        var _filter = resp["result"][t];
        if (_filter['datatype'] === 'select_list') {
            appendFilterSelectList(_filter);
        } else if (_filter['datatype'] === 'checkbox_list') {
            appendFilterCheckboxList(_filter);
        } else {
            console.warn("Unknown type of filter=", _filter);
        }
    }
}
window.available_filters = []

function initFilters(callback) {
    document.getElementById('filters').innerHTML = "";
    $.ajax({
        url: "./api/v1/available-filters",
        method: "GET"
    }).done(function(resp){
        window.available_filters = resp['result'];
        applyAllowedFilters(resp);
        if (callback) {
            callback()
        }
    }).fail(function(err){
        console.error(err)
    })
}

$(document).ready(function() {
    window.tabeditor = new TabulaturEditor('_tabulatur')
    initFilters(function() {
        // generate();
        testPart();
    })
})

function testPart() {
    var test_part_resp = {
        "guitarTuning": ["E4", "B3", "G3", "D3", "A2", "E2"], // classic
        "part":[
            {
                "time": "0/32",
                "duration": "4/32",
                "finger":"index",
                "fret":5,
                "note":"E4", // can be calculated on play use a guitar tuning
                "string":2
            }, 
            {
                "time": "4/32",
                "duration": "16/32",
                "finger":"index",
                "fret":5,
                "note":"E4",
                "string":2
            },
            {
                "time": "20/32",
                "duration":"4/32",
                "finger":"middle",
                "fret":5,
                "note":"C4",
                "string":3
            },
            {
                "time": "24/32",
                "duration":"1/4",
                "finger":"ring",
                "fret":5,
                "note":"A4",
                "string":1
            },
            {
                "time": "32/32",
                "duration":"8/32",
                "finger":"index",
                "fret":2,
                "note":"E3",
                "string":4
            },
            {
                "time": "40/32",
                "duration":"1/4",
                "finger":"index",
                "fret":3,
                "note":"F3",
                "string":4
            },
            {
                "time": "48/32",
                "duration":"1/4",
                "finger":"ring",
                "fret":5,
                "note":"G3",
                "string":4
            },
            {
                "time": "56/32",
                "duration":"1/4",
                "finger":"index",
                "fret":2,
                "note":"E3",
                "string":4
            },
            {
                "time": "64/32",
                "duration":"1/4",
                "finger":"middle",
                "fret":3,
                "note":"F3",
                "string":4
            },
            {
                "time": "72/32",
                "duration":"1/4",
                "finger":"middle",
                "fret":5,
                "note":"G3",
                "string":4
            },
            {
                "time": "80/32",
                "duration":"1/4",
                "finger":"middle",
                "fret":7,
                "note":"A3",
                "string":4
            },
            {
                "time": "88/32",
                "duration":"1/4",
                "finger":"middle",
                "fret":5,
                "note":"G3",
                "string":4
            },
            {
                "time": "96/32",
                "duration":"1/4",
                "finger":"no",
                "fret":0,
                "note":"D3",
                "string":4
            },
            {
                "time": "104/32",
                "duration":"1/4",
                "finger":"index",
                "fret":6,
                "note":"F4",
                "string":2
            },
            {
                "time": "112/32",
                "duration":"1/4",
                "finger":"index",
                "fret":5,
                "note":"E4",
                "string":2
            },
            {
                "time": "120/32",
                "duration":"1/4",
                "finger":"ring",
                "fret":7,
                "note":"A3",
                "string":4
            }
        ],
        "tabulatur":""
    };
    var t = btoa(JSON.stringify(test_part_resp));
    console.log(t.length);

    tabeditor.updateData(test_part_resp["part"]);
    tabeditor.render();
}