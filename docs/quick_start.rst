.. _quick-start-page:

Quick Start
===========


A Brief Introduction to |engine_name|
-------------------------------------

This quick start covers the basics of both how the |engine_name| game engine works and scaffolding a render/game loop.

To get started, install the |engine_name| engine:

.. code-block::

    pip install Loxoc


|engine_name| was designed with a developer experience first mindset.
If you have used game engines like `Pygame <https://www.pygame.org>`_, the structure of a basic game/render loop should look familiar.
In this tutorial you will learn how to create a render loop and add objects to it to be rendered.

To start, we'll create a new python file called ``my_game.py`` and import our dependencies.

.. code-block:: python

    # my_game.py
    from Loxoc import Camera, Window, EVENT_FLAG, Vec3
    import math

To determine what should be rendered onto the screen we need to add a :class:`Camera<Loxoc.Camera>`.

.. code-block:: python

    dim = (1280, 720,)

    camera = Camera(Vec3(0.0, 0.0, 0.0), Vec3(0.0,0.0,0.0), *dim, 10000,
        math.radians(60))

We will create the :class:`Camera<Loxoc.Camera>` at the world origin point `(0, 0, 0)` with a starting rotation of 0 degrees around the x, y, and z axis.
The dimensions of the :class:`Camera<Loxoc.Camera>`'s viewport will be 1280 by 720 or 720p resolution. It will have a focal length of 10,000 and a 60 degree field of view.

Next, we create the actual application :class:`Window<Loxoc.Window>` upon which the scene will be rendered.

.. code-block:: python

    window = Window("Loxoc Quick Start", camera, *dim, False,
        Vec3(0.2, 0.2, 0.2))

The title bar of the window will be labeled ``Loxoc Quick Start``.  We will pass in the :class:`Camera<Loxoc.Camera>` and dimensions from before
Fullscreen mode will be set to false for this example. Finally, the ambient light level is assigned a red, green, and blue color value of ``0.2``, ``0.2``, and ``0.2`` respectively.

Now we will learn how to add game assets to our :class:`Window<Loxoc.Window>` to be rendered.

.. code-block:: python

    window.add_object_list([
        # This is where we add 3D objects to be rendered to the window...
    ])

    window.add_object2d_list([
        # This is where we add 2D objects to be rendered to the window...
    ])

    window.add_point_light_list([
        # This is where we add point lights to be rendered to the scene...
    ])

In |engine_name|, game assets to be rendered are attached to the :class:`Window<Loxoc.Window>` they are to be rendered to.
This is done with explicitly named functions for each type of game asset.

Now that we have defined in our python file where we will attach game assets to our :class:`Window<Loxoc.Window>`,
we will create a basic scaffolding for our game loop.

.. code-block:: python

    while not window.event.check_flag(EVENT_FLAG.QUIT):
        # While the close button has not been clicked run the game loop:

        # game code goes here...

        window.update()
        # At the end of the game loop we update the window.

Great! Now that we have our basic game loop, we're almost ready to render something to the :class:`Window<Loxoc.Window>`.

Before we can do so, we need to learn how |engine_name|'s 3D asset or :class:`Mesh<Loxoc.Mesh>` importing system works.
It is recommended to have a ``/models/`` directory or equivalent in your project for storing your game assets.  

Here is an example of one possible project structure:

.. code-block::
    
    ./
        assets/
            models/
            sprites/
            sounds/
        my_game.py
        setup.py

        ...other project files...
    
While this example organizes its asset files inside of an ``assets/`` folder, it is not a requirement.

Before we cover how to import a 3D asset into our game, lets add one to our project:

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

There are a few **important** things to take note of here:

* The ``/textures/`` directory:

    The ``/textures/`` directory must be adjacent to the :class:`Model<Loxoc.Model>` files 
    and include all of the :class:`Model<Loxoc.Model>`'s textures.

* :class:`Model<Loxoc.Model>` file formats:

    |engine_name| supports a variety of :class:`Model<Loxoc.Model>` file formats but cannot promise support for every format.  
    While ``.gltf`` format is recommended we have also seen success with the ``.obj`` format.  
    The ``.fbx`` file format is not and will never be supported unfortunately due to it's sometimes proprietary nature.

* The ``/model_name/`` directory:

    Each :class:`Model<Loxoc.Model>` should have it's own directory to prevent name conflicts with the ``/textures/`` folder.

Now that we have our assets folder sorted out, lets add the asset to our game.  We'll need to update our imports:

.. code-block:: python

    # my_game.py
    from Loxoc import Camera, Window, EVENT_FLAG, Mesh, Object3D, MeshDict, Model
    import math

Now we will load our ``.gltf`` :class:`Model<Loxoc.Model>` from our assets folder:

.. code-block:: python

    # ... previous code

    window = Window("Loxoc Quick Start", camera, *dim, False,
        Vec3(0.2, 0.2, 0.2))
    
    my_model: Model = Model.from_file("./assets/models/model_name/model_name.gltf") # Load the model.

From the :class:`Model<Loxoc.Model>` we will create an :class:`Object3D<Loxoc.Object3D>` which can be attached to our game :class:`Window<Loxoc.Window>`.

.. code-block:: python

    my_object = Object3D(my_model, Vec3(0.0, 0.0, 20.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))
    # We create an object from the model that can be used in our game with a position of
    # x = 0, y = 0, z = 20 infront of the camera, no rotation, and a scale in all directions
    # times 1 or in other words no change in scale.
    
    window.add_object_list([
        my_object # Now we add our object to the list of objects to be rendered.
    ])

The :class:`Object3D<Loxoc.Object3D>` we created will have a starting x, y, and z position of ``0``, ``0``, and ``20`` respectively.
It will start with no initial rotation.
It's scale will be multipied by ``1.0`` in all directions. In other words, it will not be scaled at all.

Now if you run the game with...

.. code-block::

    python my_game.py

You should see your object being rendered onto the game window.

-----

Now that you've managed to render something to the :class:`Window<Loxoc.Window>`, lets explain a little bit of the API so far (see the API documentation for more details):

* :class:`Model<Loxoc.Model>` :

    :class:`Loxoc.Model` is a class which holds all of the visual information/data for an :class:`Object3D<Loxoc.Object3D>`.  It includes a :class:`MeshDict<Loxoc.MeshDict>`, animation api, and some render settings that can be tweaked to change the final appearance of a rendered model.

* :class:`MeshDict<Loxoc.MeshDict>` :

    :class:`Loxoc.MeshDict` is a class that acts as a statically typed dictionary storing each :class:`Mesh<Loxoc.Mesh>` by name.
    This is nessicary because 3D asset files can sometimes have more than one :class:`Mesh<Loxoc.Mesh>` in them.  If you have a 3D
    asset file with more than one :class:`Mesh<Loxoc.Mesh>` inside of it, you can extract them from their :class:`MeshDict<Loxoc.MeshDict>`
    to new individual :class:`MeshDict<Loxoc.MeshDict>`\s and create :class:`Model<Loxoc.Model>`\s from them to create :class:`Object3D<Loxoc.Object3D>`\s like so:

    .. code-block:: python

        my_assets: MeshDict = Model.from_file("./assets/models/model_name/model_name.gltf").mesh_dict
        # Import the 3D asset file.
        
        player_md = MeshDict("player_model_mesh", [my_assets["player_model"]])
        # Extract the Mesh into its own group/MeshDict

        player_object = Object3D(Model(player_md), Vec3(0.0, 0.0, 20.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))
        # Now our model is ready to be used.

    We can extract the :class:`Mesh<Loxoc.Mesh>`\s we need from the ``my_assets``  :class:`MeshDict<Loxoc.MeshDict>` by name.
    Hence we use ``my_assets["player_model"]``.  This is assuming your desired :class:`Mesh<Loxoc.Mesh>`
    is at the top level of your imported 3D file/asset's heirarchy.  if it is in a group inside
    the 3D file/asset you imported you could do something like:
    ``my_assets["group_name"]["player_model"]``

* :class:`Object3D<Loxoc.Object3D>`\:

    :class:`Loxoc.Object3D` is our game object.  It holds the :class:`Model<Loxoc.Model>` to be rendered, 
    the :class:`Object3D<Loxoc.Object3D>`\'s :class:`Vec3<Loxoc.Vec3>` position, it's :class:`Quaternion<Loxoc.Quaternion>` rotation, any object
    level uniforms to be used in it's :class:`Loxoc.Material` and much more.

* :meth:`Window.add_object_list()<Loxoc.Window.add_object_list>` :

    :meth:`Loxoc.Window.add_object_list` is used to add objects to the set of objects to render to the screen on :meth:`Window.update()<Loxoc.Window.update>` .
    You can think of this like creating an object in the game.  You can also remove objects with :meth:`Window.remove_object()<Loxoc.Window.remove_object>` .
    
As a final bonus for this quick start guide, we are going to make our :class:`Object3D<Loxoc.Object3D>` spin!
To do this we rotate our :class:`Object3D<Loxoc.Object3D>` arround its up direction :class:`vector<Loxoc.Vec3>`. 
It should look something like so:

.. code-block:: python

    # my_game.py
    from Loxoc import Camera, Window, EVENT_FLAG, Mesh, Object3D, MeshDict, Model, Vec3
    import math

    dim = (1280, 720,)

    camera = Camera(Vec3(0.0, 0.0, 0.0), Vec3(0.0,0.0,0.0), *dim, 10000,
        math.radians(60))
    
    window = Window("Loxoc Quick Start", camera, *dim, False,
        Vec3(0.2, 0.2, 0.2))
    
    my_model: Model = Model.from_file("./assets/models/model_name/model_name.gltf")

    my_object = Object3D(my_model, Vec3(0.0, 0.0, 20.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))
    
    window.add_object_list([
        my_object
    ])

    window.add_object2d_list([])

    window.add_point_light_list([])

    while not window.event.check_flag(EVENT_FLAG.QUIT):

        my_object.rotation.rotate(my_object.rotation.up, 10.0 * window.dt)

        window.update()

That's all there is to it!  You should now have a spinning 3D :class:`Model<Loxoc.Model>` rendering to your game :class:`Window<Loxoc.Window>`.  
Check out the :mod:`Loxoc API Reference<Loxoc>` for more info.