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
    btGImpactMeshShape *gimpact;
    btDefaultMotionState *motion_state;
    btCollisionShape* collisionshape;
    btRigidBody *body;
public:
    PhysMesh(ugfx_vertex_t *verticies, uint32_t num_verts, btDiscreteDynamicsWorld *dynWorld, btVector3 origin);
};

#endif
