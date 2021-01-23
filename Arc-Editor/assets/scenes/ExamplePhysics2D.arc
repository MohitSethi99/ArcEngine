Scene: Untitled
Entities:
  - Entity: 12837192831273
    TagComponent:
      Tag: DynamicBody1
    TransformComponent:
      Translation: [2.21107197, 3.83418465, 0]
      Rotation: [0, 0, 0.233874142]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0.772200763, 0.375665247, 0.375665247, 1]
      TextureFilepath: ""
      TilingFactor: 1
    Rigidbody2DComponent:
      Type: 2
      LinearDamping: 0
      AngularDamping: 0
      GravityScale: 1
      CollisionDetection: 0
      SleepingMode: 1
      FreezeRotationZ: false
    BoxCollider2DComponent:
      Size: [1, 1]
      Offset: [0, 0]
      IsTrigger: false
  - Entity: 12837192831273
    TagComponent:
      Tag: Camera
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 1
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: true
      FixedAspectRatio: false
  - Entity: 12837192831273
    TagComponent:
      Tag: Ground
    TransformComponent:
      Translation: [0, -3.5, 0]
      Rotation: [0, 0, 0]
      Scale: [8, 1, 1]
    SpriteRendererComponent:
      Color: [0.412543595, 0.749034762, 0.393315554, 1]
      TextureFilepath: ""
      TilingFactor: 1
    Rigidbody2DComponent:
      Type: 0
      LinearDamping: 0
      AngularDamping: 0
      GravityScale: 1
      CollisionDetection: 0
      SleepingMode: 1
      FreezeRotationZ: false
    BoxCollider2DComponent:
      Size: [1, 1]
      Offset: [0, 0]
      IsTrigger: false
  - Entity: 12837192831273
    TagComponent:
      Tag: Plank
    TransformComponent:
      Translation: [-3.12341118, 0.586266875, 0]
      Rotation: [0, 0, -2.20435095]
      Scale: [0.100000016, 2.70000052, 1]
    SpriteRendererComponent:
      Color: [0.370656371, 0.269622535, 0.143110573, 1]
      TextureFilepath: ""
      TilingFactor: 1
    Rigidbody2DComponent:
      Type: 0
      LinearDamping: 0
      AngularDamping: 0
      GravityScale: 1
      CollisionDetection: 0
      SleepingMode: 1
      FreezeRotationZ: false
    BoxCollider2DComponent:
      Size: [1, 1]
      Offset: [0, 0]
      IsTrigger: false
  - Entity: 12837192831273
    TagComponent:
      Tag: DynamicBody2
    TransformComponent:
      Translation: [-3.67118049, 4.40914488, 0]
      Rotation: [0, 0, -0.403171092]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0.282956392, 0.367442727, 0.66023165, 1]
      TextureFilepath: ""
      TilingFactor: 1
    Rigidbody2DComponent:
      Type: 2
      LinearDamping: 0
      AngularDamping: 0
      GravityScale: 1
      CollisionDetection: 0
      SleepingMode: 1
      FreezeRotationZ: false
    BoxCollider2DComponent:
      Size: [1, 1]
      Offset: [0, 0]
      IsTrigger: false
  - Entity: 12837192831273
    TagComponent:
      Tag: DynamicBody3
    TransformComponent:
      Translation: [-1.69105494, 3.57495308, 0]
      Rotation: [0, 0, 0.247836739]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0.999989986, 1, 0.999992669, 1]
      TextureFilepath: G:\CPP\ArcEngine\Arc-Editor\assets\textures\Checkerboard.png
      TilingFactor: 1
    Rigidbody2DComponent:
      Type: 2
      LinearDamping: 0
      AngularDamping: 0
      GravityScale: 1
      CollisionDetection: 0
      SleepingMode: 1
      FreezeRotationZ: false
    BoxCollider2DComponent:
      Size: [1, 1]
      Offset: [0, 0]
      IsTrigger: false
  - Entity: 12837192831273
    TagComponent:
      Tag: Sprite
    TransformComponent:
      Translation: [-2.51490927, 6.15029526, -5.96046377e-08]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
      TextureFilepath: G:\CPP\ArcEngine\Arc-Editor\assets\textures\Circle.png
      TilingFactor: 1
    Rigidbody2DComponent:
      Type: 2
      LinearDamping: 0
      AngularDamping: 0
      GravityScale: 1
      CollisionDetection: 0
      SleepingMode: 1
      FreezeRotationZ: false
    CircleCollider2DComponent:
      Radius: 0.5
      Offset: [0, 0]
      IsTrigger: false