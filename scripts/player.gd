extends CharacterBody2D

@export var ACCELERATION: float = 0.4
@export var FRICTION: float = 0.5
@export var SPEED: float = 360.0

@export var GRAVITY: float = 2500.0

func _move() -> void:
	var direction: float = Input.get_axis("A", "D")
	
	if (direction > 0.5): $Sprite2D.flip_h = false;
	elif (direction < -0.5): $Sprite2D.flip_h = true
	
	if (direction != 0.0):
		velocity.x = lerp(velocity.x, direction * SPEED, ACCELERATION)
		if (self.is_on_floor()): $AnimationPlayer.play("walk")
	else:
		velocity.x = lerp(velocity.x, 0.0, FRICTION)
		if (self.is_on_floor()): $AnimationPlayer.play("idle")
	return

func _physics_process(_delta: float) -> void:
	velocity.y += GRAVITY * _delta
	move_and_slide()
	_move()
	
	if (self.is_on_floor()):
		if (Input.is_action_pressed("jump")):
				velocity.y -= 800.0
	if (!self.is_on_floor()):
		if (velocity.y > 1.0):
			$AnimationPlayer.play("jump")
		else:
			$AnimationPlayer.play("falling")
	return
