# PebbleFace
First time making a watchface for Pebble Time


<img src="http://i.imgur.com/oDoHbtf.png" width="146" height="171" /><img src="/resources/images/prep.PNG" width="151" height="171" /><img src="/resources/images/problems.PNG" width="152" height="171" /><img src="/resources/images/water.PNG" width="147" height="171" />



Concept


The idea of this watchface is to deliver information that I find useful, when it is useful to me.
Eg at 15:30 show me upcoming bus times
16:10 Show me upcoming train times


Timeline intergration
This watchface will also pin upcoming events to my timeline so that I know what I need to do
Eg Pin task telling me to go to the gym today


Finally, I'm making this watchface open source so that others find it easy to develop for Pebble. It will be split up into small tasks as much as possible, so people can use only what's relevant to them


## Current Features
+ Colloquial time
+ Time based events (GYM, FREE TIME, MEDITATION)
+ Weather temp and condition updates / 30 minutes
+ Minimised time interface when new data needs to be rendered
+ Bus/Train times displayed at TRAVEL and PREP times
+ Live traffic incident updates using NSW transport data program
+ Timeline pins


## Planned Features
+ Reorganisation of code


## Super cool but not likely features
+ Settings support for custom events at custom times using custom colours
+ Make sense of Tripviews public facing JSON responses and get realtime transport information
+ Icons/Animations when a task changes (like a picture of weights when its gym time)
