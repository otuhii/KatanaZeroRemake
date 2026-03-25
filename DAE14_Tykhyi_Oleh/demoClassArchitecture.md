SpriteManager				  |--------Player------------|
	  ||			          |some player specific stuff|
	  ||			          | 	const Sprite*        |
	  ||				  
      ||
      \/
-----Sprite------|
|    Texture*	 |
|accumulatedTime |			  |-------Enemy--------------|		
|    frameTime   |			  | some enemy specific stuff|	
------------------			  | const Sprite*            |	
  Update(float)				  |--------------------------|
  Draw(Vector2f)			as i will have two types of enemies i will inherit those type-specific classes from this base class 
							

					
		
					  |------Camera-------|
					  | 	Aim()         |
					  |   float width     |
			          | float height      |


					  	

					  |-----PickUpItem------|
					  will be done via relationship player uses a pickup