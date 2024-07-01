.. _quick-start-page:

Quick Start
===========

How |engine_name| Works
-----------------------

|engine_name| is a very straight forward and easy to use game engine.
If you have used game engines like `Pygame <https://www.pygame.org>`_ 
things should look somewhat familiar in terms of the structure of a basic game/render loop.

To start, first lets make a basic renderloop in |engine_name|:

.. code-block:: python

    from Loxoc import Camera, Window, EVENT_FLAG
    import math

    dim = (720, 10000)

    camera = Camera(Vec3(0.0, 0.0, 0.0), Vec3(0.0,0.0,0.0), 1280, *dim,
        math.radians(60))
    # Create the camera at x = 0, y = 0, z = 0 with no rotation 720p resolution,
    # 10000 focal length, and 60 fov

    window = Window("Loxoc Quick Start", camera, *dim, False,
        Vec3(0.2, 0.2, 0.2))
    # Create a window titled "Loxoc Quick Start" with the same dimensions as the camera,
    # in windowed mode, with an ambient light level of red = 0.2, blue = 0.2, and green = 0.2
    
    window.add_object_list([
        # This is where we add 3D objects to be rendered to the scene...
    ])

    window.add_object2d_list([
        # This is where we add 2D objects to be rendered on the screen surface infront of
        # everything...
    ])

    window.add_point_light_list([
        # This is where we add point lights to be rendered to the scene...
    ])

    while not window.event.check_flag(EVENT_FLAG.QUIT):
        # While the close button has not been clicked run the game loop:

        # game code goes here...

        window.update()
        # At the end of the game loop we update the window.

Great!  Now that we have our basic game loop we can move onto the fun part.  
Lets try rendering something to the :class:`Window<Loxoc.Window>` .

Before we can render anything to the :class:`Window<Loxoc.Window>` , 
we need to learn how the 3D asset / :class:`Mesh<Loxoc.Mesh>` importing system works.  It is recommended to 
have a ``/models/`` directory or equivalent in your project for storing your game assets.  
Other than that Its pretty flexible.

Here is an example of a possible project structure:

.. code-block::
    
    ./
        assets/
            models/
            sprites/
            sounds/
        my_game.py
        setup.py

        ...other project files...
    
While this example organizes its asset files inside of an ``assets/`` folder, you dont have to.  
Now lets first add a 3D asset to this project, then we'll explain how it works:

.. code-block::

    ./
        assets/
            models/
                model_name/
                    textures/
                        model_texture.png
                    model_name.bin
                    model_name.gltf
            sprites/
            sounds/
        my_game.py
        setup.py

        ...other project files...

There are a few things that are **important** to keep in mind here:

* The ``/textures/`` directory:

    The ``/textures/`` directory is required to be adjacent to the model files 
    and must include all of the model's textures.

* Model file formats:

    The format of model should be flexible, but we cannot promise every format will work.  
    While ``.gltf`` format is recommended we have also seen success with ``.obj`` format.  
    ``.fbx`` is not and will never be supported unfortunately due to its proprietary nature.

* The ``/model_name/`` directory:

    Each model should have its own directory and texture folder to prevent name conflicts.

Now that we have our model sorted out, lets get to adding it to our game.  We'll need a few
new imports namely :class:`Loxoc.Mesh` and :class:`Loxoc.Object3D` .  We will also include :class:`Loxoc.MeshDict`
(we will explain the purpose of :class:`MeshDict<Loxoc.MeshDict>` in a minute) for type annotations.  It should look something like this:

.. code-block:: python

    # my_game.py
    from Loxoc import Camera, Window, EVENT_FLAG, Mesh, Object3D, MeshDict
    import math

    dim = (720, 10000)

    camera = Camera(Vec3(0.0, 0.0, 0.0), Vec3(0.0,0.0,0.0), 1280, *dim,
        math.radians(60))
    
    window = Window("Loxoc Quick Start", camera, *dim, False,
        Vec3(0.2, 0.2, 0.2))
    
    my_model_meshes: MeshDict = Mesh.from_file("./assets/models/model_name/model_name.gltf")
    # We import the contents from the 3D asset file into a MeshDict variable so we can use our
    # model in our game.

    my_object = Object3D(my_model_meshes, Vec3(0.0, 0.0, 20.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))
    # We create an object from the model that can be used in our game with a position of
    # x = 0, y = 0, z = 20 infront of the camera, no rotation, and a scale in all directions
    # times 1 or in other words no change in scale.
    
    window.add_object_list([
        my_object # Now we add our object to the list of objects to be rendered.
    ])

    window.add_object2d_list([])

    window.add_point_light_list([])

    while not window.event.check_flag(EVENT_FLAG.QUIT):

        window.update()

Tada! you should now see your model being rendered when you run your game.  
Now lets explain a few things:

* :class:`MeshDict<Loxoc.MeshDict>` :

    :class:`Loxoc.MeshDict` is a datastructure that acts like a statically typed dictionary storing each :class:`Mesh<Loxoc.Mesh>` by name.
    This is nessicary because 3D asset files can have more than one :class:`Mesh<Loxoc.Mesh>` in them.  If you have a 3D
    asset file with more than one :class:`Mesh<Loxoc.Mesh>` inside of it, you can extract them from their :class:`MeshDict<Loxoc.MeshDict>`
    to new individual :class:`MeshDict<Loxoc.MeshDict>` s to be used in :class:`Object3D<Loxoc.Object3D>` s like so:

    .. code-block:: python

        my_assets: MeshDict = Mesh.from_file("./assets/models/model_name/model_name.gltf")
        # Import the 3D asset file.
        
        player_model = MeshDict([my_assets["player_model"]])
        # Extract the Mesh into its own group/MeshDict

        player_object = Object3D(player_model, Vec3(0.0, 0.0, 20.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))
        # Now our model is ready to be used.

    We can extract the :class:`Mesh<Loxoc.Mesh>` s we need from the ``my_assets``  :class:`MeshDict<Loxoc.MeshDict>` by name.
    Hence we use ``my_assets["player_model"]``.  This is assuming your desired :class:`Mesh<Loxoc.Mesh>`
    is at the top level of your imported 3D file/asset's heirarchy.  if it is in a group inside
    the 3D file/asset you imported you could do something like:
    ``my_assets["group_name"]["player_model"]``

* :class:`Object3D<Loxoc.Object3D>` :

    :class:`Loxoc.Object3D` is our game object.  It holds the :class:`MeshDict<Loxoc.MeshDict>` of :class:`Mesh<Loxoc.Mesh>` es to be rendered, 
    the :class:`Object3D<Loxoc.Object3D>` 's :class:`Vec3<Loxoc.Vec3>` position, its :class:`Quaternion<Loxoc.Quaternion>` rotation, any object
    level uniforms to be used in it's :class:`Loxoc.Material` and much more.

* :meth:`Window.add_object_list()<Loxoc.Window.add_object_list>` :

    :meth:`Loxoc.Window.add_object_list` is used to add objects to the set of objects to render to the screen on :meth:`Window.update()<Loxoc.Window.update>` .
    You can think of this like creating an object in the game.  You can also remove objects with :meth:`Window.remove_object()<Loxoc.Window.remove_object>` .
    
As one final bonus of this Quick Start We are going to make our :class:`Object3D<Loxoc.Object3D>` spin!
To do this we are going to rotate our :class:`Object3D<Loxoc.Object3D>` arround its up directional vector. 
It should look something like so:

.. code-block:: python

    # my_game.py
    from Loxoc import Camera, Window, EVENT_FLAG, Mesh, Object3D, MeshDict
    import math

    dim = (720, 10000)

    camera = Camera(Vec3(0.0, 0.0, 0.0), Vec3(0.0,0.0,0.0), 1280, *dim,
        math.radians(60))
    
    window = Window("Loxoc Quick Start", camera, *dim, False,
        Vec3(0.2, 0.2, 0.2))
    
    my_model_meshes: MeshDict = Mesh.from_file("./assets/models/model_name/model_name.gltf")

    my_object = Object3D(my_model_meshes, Vec3(0.0, 0.0, 20.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))
    
    window.add_object_list([
        my_object
    ])

    window.add_object2d_list([])

    window.add_point_light_list([])

    while not window.event.check_flag(EVENT_FLAG.QUIT):

        my_object.rotation.rotate(my_object.rotation.up, 10.0 * window.dt)

        window.update()

And thats all there is to it!  You should now have a spinning 3D model rendering on your game window.  
Check out the :mod:`Loxoc API Reference<Loxoc>` for more info.