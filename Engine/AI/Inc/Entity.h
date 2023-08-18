#pragma once


namespace AI
{
	class AIWorld;
	class Entity;

	using EntityPtrs = std::vector<Entity*>;

	class Entity
	{
	public:
		Entity(AIWorld& world, uint32_t typeId);
		virtual ~Entity();

		Entity(const Entity&) = delete;
		Entity(const Entity&&) = delete;
		Entity& operator=(const Entity&) = delete;
		Entity& operator=(const Entity&&) = delete;

		AIWorld& world;
		X::Math::Vector2 position = X::Math::Vector2::Zero();
		X::Math::Vector2 heading = X::Math::Vector2::YAxis();

		X::Math::Matrix3 GetWorldTransform() const;

		uint32_t GetTypeId() const { return static_cast<uint32_t>(mUniqueID >> 32); }
		uint64_t GetUniqueId() const { return mUniqueID; }

	private:
		const uint64_t mUniqueID = 0;
	};
}