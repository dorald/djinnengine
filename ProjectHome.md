Currently, D'Jinn features includes:

  * Test Game: A class for Static Unit Tests

  * Actor System: Handles any actors within the game, and will be the base class for Characters / NPCs / Enemies / Player / Game Objects etc. Used Angel-Engine Actor as a template.

  * Animation System: An animation controller used for stopping, starting, pausing, and managing a sprites various animations.

  * Game Screen Management: Used to manage simple game flow by maintaining one or more game screen instances. Through the Screen Controller class, the game has a way to update and render all active screens even if they are not in focus, and determine which game screen should be given any input that needs to be processed.

  * Serializable Objects: All game objects (Characters, Particle Effects, Levels, etc) will be serialized in .dat files for dynamic loading. This way they do not need to be hard coded into the engine, and the engine can be re-used for various projects and is not data specific.

  * Input Management (Single Touch Only Currently): The Input Manager manages various states including the currentState, previousState, and upcomingState. Through these various input states, it can be determined where the touch is happening (or IF the screen is being touched) and each game screen can take this information and process it however it needs.

  * Particle System: A particle controller maintains a list of available effects, and any active effects. Every particle effect contains one or more particle emitters, and through the Particle Controller, each emitter will be updated and when the effect is complete, clean up any memory allocated.

  * OpenAL Sound Support: Currently a very crude engine using OpenAL for sound effects. Eventually, this will be modified for the use of OGG files and music streaming.

  * File Management: Pass in the asset name of a file, and a dereferenced pointer to where that object will live, and let the File Manager deserialize the object for you.

  * Texture Controller: A class that will handle all memory for textures, and if various classes are using the same texture, it will simply hand out references to the active texture instead of creating multiple copies of it. An example is in a forrest, if there are 30 trees that re-use the same texture, it wouldn't make sense to allocate 30 instances of the texture, just instantiate one and hand out 30 references.

  * Batch Texture Rendering: The texture controller will batch together all sprites wanting to be rendered to screen for a given frame, and separate them by texture GLuint. A single draw call is made for OpenGL per texture to save processing power, then the batch list is cleared and a new batch is created from the client wishing to render.

  * Textue2D: Re-written Texture class that has roughly the same interface as the Apple Texture2D, but works with the texture management system. Does not include handling of non-power of 2 textures, but does handle reference counting. If more than 1 object is using this texture, it will know.. when when the reference count reaches 0, it will deallocate itself.

  * Math Utilities: Various helper functions like Clamp, Min, Max, Abs, Lerp, etc. Heavily influenced by the Angel-Engine Math Utilities class.

D'Jinn was started as a platform to learn the C++ programming language in an environment that I found entertaining. Because of this goal, it was deemed appropriate to turn the D'Jinn engine into an open source engine for the community to use, poke through, and even modify. If you have any questions, comments, or would like to help contribute, feel free to send me an email and we'll chat. If you would like to take what is currently here, and use it in your project, I wish you the best of luck and ask that if you make any improvements, that they be shared with the community.

The D'Jinn Engine is an ongoing project, and will be used to power my up-coming, un-announced game. If there is a feature that you would like to request, feel free to drop by the discussion forums.

This engine is a tribute to the few of us who actually enjoy C++ enough to use it on the iPhone. Thank you everyone, for I have learned so much in so little time.

Craig Giles
http://www.CraigGiles.com/