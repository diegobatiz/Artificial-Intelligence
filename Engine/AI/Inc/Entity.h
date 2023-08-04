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

		uint32_t GetTypeId() const { return static_cast<uint32_t>(mUniqueID >> 32); }

	private:
		const uint64_t mUniqueID = 0;
	};
}