﻿using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;

namespace bumblegumpers
{
	public struct InputState
	{
		public bool leftPressed, rightPressed, upPressed, downPressed, attackPressed, usePressed, shieldPressed;
		public KeyboardState currentKeyboard, pastKeyboard;
		public MouseState currentMouse, pastMouse;
	}

	public class BumbleGumpers : Game
	{
		Player player;
		World world;

		Render renderer;

		GraphicsDeviceManager graphics;

		InputState input;

		Editor editor;

		bool inEditor = false;

		public BumbleGumpers()
		{
			IsMouseVisible = true;
			Window.Title = "?????";
			IsFixedTimeStep = true;
			TargetElapsedTime = new TimeSpan(166667);
			
			graphics = new GraphicsDeviceManager(this);
			graphics.PreferredBackBufferWidth = 768;
			graphics.PreferredBackBufferHeight = 576;
			graphics.ApplyChanges();
			Content.RootDirectory = "Content";
		}

		protected override void Initialize()
		{
			renderer = new Render(this);

			editor = new Editor(renderer);

			world = new World(16, 12);
			editor.setWorld(world);
			player = new Player(1, 10);
			input.pastKeyboard = Keyboard.GetState();
			input.pastMouse = Mouse.GetState();

			this.Window.ClientSizeChanged += new EventHandler<EventArgs>(WindowResize);
			base.Initialize();
		}

		protected override void LoadContent()
		{
			renderer.loadGraphics(Content);
		}

		protected override void UnloadContent()
		{
			
		}

		void WindowResize(object sender, EventArgs e)
		{
			if (inEditor)
				editor.resize(this.Window.ClientBounds);
			renderer.refreshCamera();
		}

		protected override void Update(GameTime gameTime)
		{
			if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
				Exit();
			updateInput();

			bool toggleEdit;
			updateKey(Keys.E, out toggleEdit);
			if (toggleEdit)
			{
				inEditor = !inEditor;
				if (inEditor)
				{
					graphics.PreferredBackBufferWidth = 1400;
					graphics.PreferredBackBufferHeight = 900;
					graphics.ApplyChanges();
					Window.AllowUserResizing = true;
					renderer.refreshCamera();
				}
				else
				{
					graphics.PreferredBackBufferWidth = 768;
					graphics.PreferredBackBufferHeight = 576;
					graphics.ApplyChanges();
					Window.AllowUserResizing = false;
					renderer.refreshCamera();
				}
			}

			if (!inEditor)
				player.update(input, world);
			else
				editor.update(input);

			input.pastMouse = input.currentMouse;
			input.pastKeyboard = input.currentKeyboard;
			base.Update(gameTime);
		}

		void updateKey(Keys key, out bool val)
		{
			if (input.currentKeyboard.IsKeyDown(key) && input.pastKeyboard.IsKeyUp(key))
				val = true;
			else
				val = false;
		}

		protected override void Draw(GameTime gameTime)
		{
			GraphicsDevice.Clear(Color.Black);

			if (inEditor)
				editor.renderEditor();
			else
			{
				renderer.startRender();
				renderer.renderWorld(world);
				renderer.renderPlayer(player);
				renderer.endRender();
			}

			base.Draw(gameTime);
		}

		void updateInput()
		{
			input.currentMouse = Mouse.GetState();
			input.currentKeyboard = Keyboard.GetState();
			updateKey(Keys.Left, out input.leftPressed);
			updateKey(Keys.Right, out input.rightPressed);
			updateKey(Keys.Up, out input.upPressed);
			updateKey(Keys.Down, out input.downPressed);
			updateKey(Keys.Z, out input.attackPressed);
			updateKey(Keys.X, out input.usePressed);
			updateKey(Keys.C, out input.shieldPressed);
		}
	}
}