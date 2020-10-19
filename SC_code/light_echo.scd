Server.local

o = Server.default.options

ServerOptions.

OSCFunc


NetAddr.langPort


OSCFunc.trace(bool: true, hideStatusMsg: false)


OSCdef(\setup,{|msg,time,addr,recvPort|
	">>>>>setup<<<<<".postln;
	msg.postln;
	s.sendMsg(\s_new,\light_echo,1000+msg[1],0,0,\freq,330);
},'/setup');

OSCdef(\update,{|msg,time,addr,recvPort|
	"update".postln;
	msg.postln;
},'/update');

OSCdef(\kill,{|msg,time,addr,recvPort|
	">>>>>kill<<<<<".postln;
	msg.postln;
	s.sendMsg(\n_free,1000+msg[1]);
},'/kill');


s.sendMsg(\s_new,\light_echo,1001,0,0,\freq,330,\dur,3);
s.sendMsg(\n_set,1001,\freq,550);
s.sendMsg(\n_set,1001,\freq,770);
s.sendMsg(\n_set,1001,\freq,770);
s.sendMsg(\n_free,1001);



OSCdef.freeAll;

SynthDef(\light_echo, {|amp=0.5, pan=0, freq=220, out=0,dur=1|
	Out.ar(out,
		Pan2.ar(
			SinOsc.ar(freq,0,amp)*Line.kr(1,0,dur, doneAction: Done.freeSelf),
			pan, 0.5);
	);
}).add

Synth.new(\light_echo,[\freq,330,\pan,0,\dur,0.1])

n = s.nextNodeID;
s.sendMsg(\s_new,\light_echo,12,0,0,\freq,330);



s.sendMsg(\new_spot)

5.postln



n = NetAddr("127.0.0.1", 57120); // local machine

OSCdef(\test, {|msg, time, addr, recvPort| \unmatching.postln}, '/chat', n); // def style
OSCdef.newMatching(\test2, {|msg, time, addr, recvPort| \matching.postln}, '/chat', n); // path matching
OSCdef(\test3, {|msg, time, addr, recvPort| \oneShot.postln}, '/chat', n).oneShot; // once only


m = NetAddr("127.0.0.1", 57120); // loopback

m.sendMsg("/chat", "Hello App 1");
m.sendMsg("/chat", "Hello App 1"); // oneshot gone
m.sendMsg("/ch?t", "Hello App 1");
m.sendMsg("/*", "Hello App 1");
m.sendMsg("/chit", "Hello App 1"); // nothing