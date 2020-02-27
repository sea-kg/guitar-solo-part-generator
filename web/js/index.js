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

window.soloData = [];

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
    renderTabulatur("_tabulatur", window.soloData, window.guitarTunings, idx + 1);
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
}

function play() {
    stop();

    updateTabulaturNoteEnded(-1);

    window.ac = new AudioContext()
    window.wave = waves['piano'];
    var dcn = ac.createDynamicsCompressor();
    dcn.ratio = 16;
    dcn.attack = 0.0005;
    dcn.connect(ac.destination);
    window.out = ac.createGain();
    window.out.connect(dcn);

    var intrument = $('#use_instrument').val();
    window.wave = waves[intrument];
    for (var i = 0; i < window.soloData.length; i++) {
        addNote(window.soloData[i], i*0.3, 0.25, i*0.3, i);
    }  
}

window.guitarTunings = ["E4", "B3", "G3", "D3", "A2", "E2"]

function generate() {
    stop();
    var elems = $('.gspg-filter')
    var filters = {}
    for (var i = 0; i < elems.length; i++) {
        var el = $(elems[i])
        var filter_name = el.attr('filter-name');
        var filter_value = el.val();
        filters[filter_name] = filter_value;   
    }

    $.ajax({
        url: "/api/v1/solo-generate",
        method: "GET",
        data: filters
    }).done(function(resp){
        $('#tabulatur').html(resp['tabulatur']);
        window.soloData = resp["part"];
        renderTabulatur("_tabulatur", window.soloData, window.guitarTunings)
        console.log(window.soloData)
    }).fail(function(err){
        console.error(err)
        window.soloData = [];

        renderTabulatur("_tabulatur", window.soloData, window.guitarTunings)
        $('#tabulatur').html(err.statusText);
    })
}

function changedValueOfList(e) {
    var name = e.attributes['filter-name'].nodeValue
    pageParams[name] = e.value
    changeLocationState(pageParams)
}

function appendFilterList(_filter) {
    var name = _filter["name"];
    var pre_val = "";
    if (containsPageParam(name)) {
        pre_val = pageParams[name]
    } else {
        pre_val = _filter["values"][0]["value"]
    }
    var _content = ''
        + '<div class="input-group mb-3" id="min_fret_filter_parent">'
        + '  <div class="input-group-prepend">'
        + '    <label class="input-group-text" for="min_fret_filter">' + _filter["caption"] + '</label>'
        + '  </div>'
        + '  <select class="custom-select gspg-filter" filter-name="' + name + '" id="filter_' + name + '" onchange="changedValueOfList(this)">'
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

function applyAllowedFilters(resp) {
    for (var t in resp["result"]) {
        var _filter = resp["result"][t];
        if (_filter['datatype'] === 'list') {
            appendFilterList(_filter);
        } else {
            console.warn("Unknown type of filter=", _filter);
        }
    }
}

function initFilters(callback) {
    document.getElementById('filters').innerHTML = "";
    $.ajax({
        url: "/api/v1/available-filters",
        method: "GET"
    }).done(function(resp){
        applyAllowedFilters(resp);
        if (callback) {
            callback()
        }
    }).fail(function(err){
        console.error(err)
    })
}

$(document).ready(function() {
    initFilters(function() {
        generate();
    })
})