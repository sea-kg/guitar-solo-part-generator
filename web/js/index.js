// https://github.com/stuartmemo/qwerty-hancock/blob/master/dist/qwerty-hancock.js

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
function stopAllNodes() {
    for (var i = 0; i < nodes.length; i++) {
        nodes[i].stop(0);
        nodes[i].disconnect();
    }
    nodes = []
}

window.AudioContext = window.AudioContext || window.webkitAudioContext;
var context = new AudioContext()

function addNote(note) {
    masterGain = context.createGain();
    masterGain.gain.value = 0.5;
    masterGain.connect(context.destination); 
    
    var oscillator = context.createOscillator();
    oscillator.type = 'sawtooth'; // 'square';
    oscillator.frequency.value = getFrequencyOfNote(note);
    oscillator.connect(masterGain);
    oscillator.start(0);
    nodes.push(oscillator);
}

var interval = null;
function play() {
    var i = 0;
    addNote(window.soloData[0].note);
    interval = setInterval(function() {
        stopAllNodes();
        i++;
        if (i < window.soloData.length) {
            addNote(window.soloData[i].note);
        } else {
            clearInterval(interval);
        }
    }, 200);
        // keyboard = new QwertyHancock(settings);        
}
function stop() {
    clearInterval(interval);
    stopAllNodes();
}
/*
Частоты открытых струн:
6-я 82,4 Гц    getFrequencyOfNote("E2");
5-я 110 Гц     getFrequencyOfNote("A2");
4-я 146,8 Гц   getFrequencyOfNote("D3");
3-я 195,99 Гц  getFrequencyOfNote("G3");
2-я 246,9 Гц   getFrequencyOfNote("B3");
1-я 329,6 Гц   getFrequencyOfNote("E4");
*/

function generate() {
    stop();

    $.ajax({
        url: "/api/v1/solo-generate",
        method: "GET"
    }).done(function(resp){
        $('#tabulatur').html(resp['tabulatur']);
        window.soloData = resp["part"];
    }).fail(function(err){
        console.error(err)
    })
}

$(document).ready(function() {
    
    generate();
})