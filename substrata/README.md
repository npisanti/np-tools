
sampler arguments ( address `/a` - `/l` )
- sample 
- pitch 
- position
- pan 
- attack
- release
- feedback
- comb pitch
( if sample is not given the sample is not retriggered but pitch, pan, feedback and comb pitch can still be processed )

wavatable synth arguments ( address `/1` - `/9` )
- wave 
- pitch
- attack in frames
- release in 2xframes
- envelope amount to cutoff
- envelope amount to wave 
- octaves added to cutoff 

zap synth arguments ( address `/z` )
- pitch
- envelope release time
- envelope amount to pitch

noise synth arguments ( address `/x` )
- pitch
- envelope release time
- envelope to filter mod amount
- decimation 

fm sub synth arguments ( address `/s` )
- pitch
- hold in frames
- feedback
- growl 
( if just pitch is given it just changes the pitch without retrigger )
