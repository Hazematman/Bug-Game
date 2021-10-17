#ifndef PHYSMESH_HPP
#define PHYSMESH_HPP
#include <libdragon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletCollision/Gimpact/btGImpactShape.h>

class PhysMesh
{
private:
    btTriangleMesh *trimesh;
    btCollisionShape *col_shape;
    btGImpactMeshShape *gimpact;
    btDefaultMotionState *motion_state;
public:
    PhysMesh(ugfx_vertex_t *verticies, uint32_t num_verts, btDiscreteDynamicsWorld *dynWorld, 
             btVector3 origin, uint32_t bitmask=0, uint32_t group=0);
    PhysMesh(float mass, float radius, btDiscreteDynamicsWorld *dynWorld, btVector3 origin, uint32_t bitmask=0, uint32_t group=0);
    PhysMesh(float mass, btVector3 extents, btDiscreteDynamicsWorld *dynWorld, btVector3 origin, 
             uint32_t bitmask=0, uint32_t group=0);

    btRigidBody *body;
};

#endif
