// part of copy from https://github.com/serebrov/web-audio-tests
function createWaves(ac) {
    return (function(wavedata) {
      function makePeriodicWave(data) {
        var n = data.real.length,
            real = new Float32Array(n),
            imag = new Float32Array(n),
            j;
        for (j = 0; j < n; ++j) {
          real[j] = data.real[j];
          imag[j] = data.imag[j];
        }
        try {
          // Latest API naming.
          return ac.createPeriodicWave(real, imag);
        } catch (e) { }
        try {
          // Earlier API naming.
          return ac.createWaveTable(real, imag);
        } catch (e) { }
        return null;
      }
      function makeMultiple(data, mult, amt) {
        var result = { real: [], imag: [] }, j, n = data.real.length, m;
        for (j = 0; j < n; ++j) {
          m = Math.log(mult[Math.min(j, mult.length - 1)]);
          result.real.push(data.real[j] * Math.exp(amt * m));
          result.imag.push(data.imag[j] * Math.exp(amt * m));
        }
        return result;
      }
      var result = {}, k, d, n, j, ff, record, wave, pw;
      for (k in wavedata) {
        d = wavedata[k];
        wave = makePeriodicWave(d);
        if (!wave) { continue; }
        record = { wave: wave };
        // A strategy for computing higher frequency waveforms: apply
        // multipliers to each harmonic according to d.mult.  These
        // multipliers can be interpolated and applied at any number
        // of transition frequencies.
        if (d.mult) {
          ff = wavedata[k].freq;
          record.freq = {};
          for (j = 0; j < ff.length; ++j) {
            wave =
              makePeriodicWave(makeMultiple(d, d.mult, (j + 1) / ff.length));
            if (wave) { record.freq[ff[j]] = wave; }
          }
        }
        // This wave has some default filter settings.
        if (d.defs) {
          record.defs = d.defs;
        }
        result[k] = record;
      }
      return result;
    })({
      // Currently the only nonstandard waveform is "piano".
      // It is based on the first 32 harmonics from the example:
      // https://github.com/GoogleChrome/web-audio-samples
      // /blob/gh-pages/samples/audio/wave-tables/Piano
      // That is a terrific sound for the lowest piano tones.
      // For higher tones, interpolate to a customzed wave
      // shape created by hand, and apply a lowpass filter.
      piano: {
        real: [0, 0, -0.203569, 0.5, -0.401676, 0.137128, -0.104117, 0.115965,
               -0.004413, 0.067884, -0.00888, 0.0793, -0.038756, 0.011882,
               -0.030883, 0.027608, -0.013429, 0.00393, -0.014029, 0.00972,
               -0.007653, 0.007866, -0.032029, 0.046127, -0.024155, 0.023095,
               -0.005522, 0.004511, -0.003593, 0.011248, -0.004919, 0.008505],
        imag: [0, 0.147621, 0, 0.000007, -0.00001, 0.000005, -0.000006, 0.000009,
               0, 0.000008, -0.000001, 0.000014, -0.000008, 0.000003,
               -0.000009, 0.000009, -0.000005, 0.000002, -0.000007, 0.000005,
               -0.000005, 0.000005, -0.000023, 0.000037, -0.000021, 0.000022,
               -0.000006, 0.000005, -0.000004, 0.000014, -0.000007, 0.000012],
        // How to adjust the harmonics for the higest notes.
        mult: [1, 1, 0.18, 0.016, 0.01, 0.01, 0.01, 0.004,
                  0.014, 0.02, 0.014, 0.004, 0.002, 0.00001],
        // The frequencies at which to interpolate the harmonics.
        freq: [65, 80, 100, 135, 180, 240, 620, 1360],
        // The default filter settings to use for the piano wave.
        // TODO: this approach attenuates low notes too much -
        // this should be fixed.
        defs: { wave: 'piano', gain: 0.5,
                attack: 0.002, decay: 0.25, sustain: 0.03, release: 0.1,
                decayfollow: 0.7,
                cutoff: 800, cutfollow: 0.1, resonance: 1, detune: 0.9994 }
      },
      bass: {
         real: [
           0.000000, -0.000001, -0.085652, 0.034718, -0.036957, 0.014576,
  -0.005792, 0.003677, -0.002998, 0.001556, -0.000486, 0.001500,
  -0.000809, 0.000955, -0.000169, 0.000636, -0.000682, 0.000663,
  -0.000166, 0.000509, -0.000420, 0.000194, -0.000025, 0.000267,
  -0.000299, 0.000226, -0.000038, 0.000163, -0.000273, 0.000141,
  -0.000047, 0.000109],
         imag: [
          0.000000, 0.500000, -0.000001, 0.000000, -0.000001, 0.000001,
          -0.000000, 0.000000, -0.000000, 0.000000, -0.000000, 0.000000,
          -0.000000, 0.000000, -0.000000, 0.000000, -0.000000, 0.000000,
          -0.000000, 0.000000, -0.000000, 0.000000, -0.000000, 0.000000,
          -0.000000, 0.000000, -0.000000, 0.000000, -0.000000, 0.000000,
          -0.000000, 0.000000],
        // How to adjust the harmonics for the higest notes.
        mult: [1, 1, 0.18, 0.016, 0.01, 0.01, 0.01, 0.004,
                  0.014, 0.02, 0.014, 0.004, 0.002, 0.00001],
        // The frequencies at which to interpolate the harmonics.
        freq: [65, 80, 100, 135, 180, 240, 620, 1360],
        // The default filter settings to use for the piano wave.
        // TODO: this approach attenuates low notes too much -
        // this should be fixed.
        defs: { wave: 'bass', gain: 0.5,
                attack: 0.002, decay: 0.25, sustain: 0.03, release: 0.1,
                decayfollow: 0.7,
                cutoff: 800, cutfollow: 0.1, resonance: 1, detune: 0.9994 }
      },
      guitar: {
         real: [
            0.000000, -0.000000, -0.179748, 0.252497, -0.212162, 0.069443,
            -0.067304, 0.006291, -0.063344, 0.007604, -0.069661, 0.004429,
            -0.019030, 0.000601, -0.001895, 0.000841, -0.009026, 0.001311,
            -0.024059, 0.002217, -0.019063, 0.002118, -0.048490, 0.000659,
            -0.007014, 0.000529, -0.003632, 0.000157, -0.000265, 0.000046,
            -0.000325, 0.000503],
         imag: [
            0.000000, 0.208930, -0.000001, 0.000004, -0.000005, 0.000003,
            -0.000004, 0.000000, -0.000006, 0.000001, -0.000010, 0.000001,
            -0.000004, 0.000000, -0.000001, 0.000000, -0.000003, 0.000001,
            -0.000012, 0.000001, -0.000011, 0.000001, -0.000035, 0.000001,
            -0.000006, 0.000000, -0.000004, 0.000000, 0.000000, 0.000000,
            0.000000, 0.000001],
        // How to adjust the harmonics for the higest notes.
        mult: [1, 1, 0.18, 0.016, 0.01, 0.01, 0.01, 0.004,
                  0.014, 0.02, 0.014, 0.004, 0.002, 0.00001],
        // The frequencies at which to interpolate the harmonics.
        freq: [65, 80, 100, 135, 180, 240, 620, 1360],
        // The default filter settings to use for the piano wave.
        // TODO: this approach attenuates low notes too much -
        // this should be fixed.
        defs: { wave: 'guitar', gain: 0.5,
                attack: 0.002, decay: 0.25, sustain: 0.03, release: 0.1,
                decayfollow: 0.7,
                cutoff: 800, cutfollow: 0.1, resonance: 1, detune: 0.9994 }
      },
      trombone: {
         real: [
  0.000000, 0.171738, 0.131907, -0.194800, -0.129913, -0.081043,
  0.049213, 0.027828, -0.008357, -0.005044, 0.002145, 0.000773,
  -0.001392, -0.000916, -0.000012, 0.000323, -0.000003, 0.000127,
  -0.000135, -0.000029, -0.000031, 0.000087, -0.000091, 0.000005,
  -0.000026, 0.000027, -0.000062, -0.000017, -0.000002, 0.000002,
  0.000012, -0.000024
         ],
         imag: [
   0.000000, -0.090905, 0.482287, 0.259485, 0.009402, -0.125271,
  -0.046816, 0.007872, 0.001762, -0.010488, -0.002305, 0.001791,
  0.001101, -0.000303, -0.000064, 0.000143, 0.000059, 0.000116,
  0.000120, -0.000011, -0.000066, -0.000019, 0.000024, 0.000014,
  0.000069, 0.000056, 0.000005, 0.000002, -0.000026, -0.000015,
  0.000055, 0.000012
  ],
        // How to adjust the harmonics for the higest notes.
        mult: [1, 1, 0.18, 0.016, 0.01, 0.01, 0.01, 0.004,
                  0.014, 0.02, 0.014, 0.004, 0.002, 0.00001],
        // The frequencies at which to interpolate the harmonics.
        freq: [65, 80, 100, 135, 180, 240, 620, 1360],
        // The default filter settings to use for the piano wave.
        // TODO: this approach attenuates low notes too much -
        // this should be fixed.
        defs: { wave: 'trombone', gain: 0.5,
                attack: 0.002, decay: 0.25, sustain: 0.03, release: 0.1,
                decayfollow: 0.7,
                cutoff: 800, cutfollow: 0.1, resonance: 1, detune: 0.9994 }
      },
    });
  }