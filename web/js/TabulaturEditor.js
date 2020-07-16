
window.lastSelectedPosition = {str: -1, fret: 0, absTime: 0};

function TabulaturEditor(elementId) {
    var self = this;
    self.elementId = elementId;
    self.data = [];
    self.countOfStirngs = 6;
    self.guitarTunings = ["E4", "B3", "G3", "D3", "A2", "E2"];  // classic

    self.frame = 0;
    self.updateData = function(newData) {
        self.data = []
        self.data = newData;
    }
    self.sirealizeToString = function() {
        var s = "[[" + self.guitarTunings.join(",") + "]";
        for (var i = 0; i < self.data.length; i++) {
            var note = self.data[i];
            s += ",[" + note.finger.toUpperCase()[0] + "," + note.string + "," + note.fret + "," + note.duration + "]"
        }
        s += "]";
        return s;
    }

    self.render = function(selectedNote) {
        self.frame++;
        // console.log("render frame#" + self.frame);
        var canvas = document.getElementById(self.elementId);
        var ctx = canvas.getContext('2d');
        ctx.lineWidth = 1;
        ctx.fillStyle = 'rgb(255, 255, 255)';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        var noteOneDurationPx = 40;
        var countStrings = self.guitarTunings.length;
        var countTakts = 4;
        var heightBetweenStrings = 18;
        var taktWidth = noteOneDurationPx * 5;
        var taktHeight = (countStrings -1) * heightBetweenStrings;
        var paddingLeft = 40;
        var paddingTop = 60;
        
        var fontSize = 12;
        ctx.font = fontSize + "px Arial";
        ctx.strokeStyle = 'rgb(10, 10, 10)';
        ctx.fillStyle = "black";
    
        // Legend
        ctx.fillText(
            "N - No finger / Empty string; I - Index finger; M - Middle finger; R - Ring finger; L - Little finger", 
            paddingLeft + 5, 
            fontSize + 5
        );
    
        for (var n = 0; n < countStrings; n++) {
            ctx.fillText((n+1) + " | " + self.guitarTunings[n], 5, paddingTop + n*heightBetweenStrings + fontSize/3);
        }
    
        ctx.fillStyle = "red";
        for (var i = 0; i <= countTakts; i++) {
            var x = paddingLeft + i*taktWidth;
            // ctx.strokeRect(x, 20, taktWidth, taktHeight);
            ctx.beginPath();
            ctx.moveTo(x, paddingTop)
            ctx.lineTo(x, paddingTop + taktHeight)
            ctx.stroke();
    
            if (i < countTakts) {
                var taktLabel = "" + (i + 1)
                ctx.fillText(taktLabel, x + 10, paddingTop - 5);
            }
        }
    
        for (var i = 0; i < countTakts; i++) {
            var x = paddingLeft + i*taktWidth;
            for (var n = 0; n < countStrings; n++) {
                ctx.beginPath();
                ctx.moveTo(x, paddingTop + n*heightBetweenStrings)
                ctx.lineTo(x + taktWidth, paddingTop + n*heightBetweenStrings)
                ctx.stroke();
            }
        }
    
        
        var shortFingers = {
            "index": "I",
            "no": "N",
            "ring": "R",
            "middle": "M",
            "little": "L",
        }
    
        var fontSize = 14;
        ctx.font = fontSize + "px Arial";
        for (var i in self.data) {
            i = parseInt(i)
            var note = self.data[i];
            var fret = note["fret"]
            var txt = "" + fret;
            var mTxt = ctx.measureText(txt);
            var nStr = note["string"]
            
            var pos = i + 1 + Math.floor(i / 4);
            var x = paddingLeft + pos * noteOneDurationPx;
            var y = paddingTop + (nStr - 1) * heightBetweenStrings;
            ctx.strokeStyle = 'rgb(10, 10, 10)';
            
            var rect_x = x - mTxt.width/2 - 2;
            var rect_y = y - fontSize/2 - 2;
            var rect_w = mTxt.width + 4;
            var rect_h = fontSize + 4;
            ctx.fillStyle = 'rgb(255, 255, 255)';
            ctx.fillRect(rect_x, rect_y, rect_w, rect_h);
            if (selectedNote == i) {
                ctx.fillStyle = 'rgb(255, 247, 0)';
                ctx.fillRect(rect_x, rect_y, rect_w, rect_h);
                ctx.strokeRect(rect_x, rect_y, rect_w, rect_h);
            }
            ctx.fillStyle = 'rgb(0, 0, 0)';
    
            var txt_y = y + fontSize / 3;
            ctx.fillText(txt, x - mTxt.width / 2, txt_y);
    
            ctx.beginPath();
            ctx.moveTo(x, paddingTop + taktHeight + heightBetweenStrings)
            ctx.lineTo(x, paddingTop + taktHeight + heightBetweenStrings*3)
            ctx.stroke();
    
            var finger = note["finger"];
            finger = shortFingers[finger];
    
            var mTxt2 = ctx.measureText(finger);
            ctx.fillText(finger, x - mTxt2.width / 2, paddingTop - fontSize);
        }

        return self.frame;
    }
    return self;
}