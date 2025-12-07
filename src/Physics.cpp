
#include "Physics.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Create Bullet collision shape from OBJ (all meshes combined)

btRigidBody* Physics::createStaticCollisionShapeFromObj(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
    if (!scene || !scene->HasMeshes()) {
        throw std::runtime_error("Failed to load OBJ: " + path);
    }


    btTriangleMesh* triMesh = new btTriangleMesh();
    for (unsigned m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];
        for (unsigned i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& face = mesh->mFaces[i];
            if (face.mNumIndices == 3) {
                aiVector3D v0 = mesh->mVertices[face.mIndices[0]];
                aiVector3D v1 = mesh->mVertices[face.mIndices[1]];
                aiVector3D v2 = mesh->mVertices[face.mIndices[2]];
                triMesh->addTriangle(btVector3(v0.x, v0.y, v0.z),
                                     btVector3(v1.x, v1.y, v1.z),
                                     btVector3(v2.x, v2.y, v2.z));
            }
        }
    }


    btBvhTriangleMeshShape* collisionShape = new btBvhTriangleMeshShape(triMesh, true);
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, collisionShape, btVector3(0, 0, 0));
    return new btRigidBody(rbInfo);
}

Physics::Physics(float gravity, MC& mc)
    : gravity_(gravity), velocity_(0, 0, 0) {


    broadphase_ = new btDbvtBroadphase();
    collisionConfig_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(collisionConfig_);
    solver_ = new btSequentialImpulseConstraintSolver();
    dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collisionConfig_);
    dynamicsWorld_->setGravity(btVector3(0, -gravity_, 0));

    std::cout << "[INFO] Bullet initialized with gravity: " << gravity_ << std::endl;

    // Add map collision

    try {
        btRigidBody* mapBody = createStaticCollisionShapeFromObj("models/ArgentinaTrack/source/PistaArgentina2.obj");
        dynamicsWorld_->addRigidBody(mapBody);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Map collision failed: " << e.what() << std::endl;
    }


    // // Ground plane
    // btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), groundLevel_);
    // btDefaultMotionState* groundMotionState =
    //     new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, groundLevel_, 0)));
    // btRigidBody::btRigidBodyConstructionInfo groundCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    // dynamicsWorld_->addRigidBody(new btRigidBody(groundCI));

    // Motorcycle rigid body

    // Motorcycle rigid body
    btCollisionShape* mcShape = new btBoxShape(btVector3(2, 1, 4)); // Larger box for bike
    btVector3 startPos(mc.getPosition().x, mc.getPosition().y, mc.getPosition().z);
    btDefaultMotionState* mcMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startPos));
    btScalar mass = 300;
    btVector3 inertia(0, 0, 0);
    mcShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo mcCI(mass, mcMotionState, mcShape, inertia);
    mcRigidBody_ = new btRigidBody(mcCI);
    mcRigidBody_->setFriction(0.8f);
    mcRigidBody_->setActivationState(DISABLE_DEACTIVATION);
    mcRigidBody_->setGravity(btVector3(0, -gravity_, 0));

    mcRigidBody_->setDamping(0.0f, 0.0f); // Remove damping

    // Enable CCD for fast movement
    mcRigidBody_->setCcdMotionThreshold(0.0001);
    mcRigidBody_->setCcdSweptSphereRadius(0.5f);
    mcRigidBody_->setContactProcessingThreshold(BT_LARGE_FLOAT);
    dynamicsWorld_->addRigidBody(mcRigidBody_);
}



    // mcRigidBody_->setActivationState(DISABLE_DEACTIVATION); // Prevent sleeping
    // mcRigidBody_->setGravity(btVector3(0, -gravity_, 0));   // Force gravity on this body
    // std::cout << "[DEBUG] Gravity applied to bike: " << mcRigidBody_->getGravity().y() << std::endl;
    // dynamicsWorld_->addRigidBody(mcRigidBody_);
    //
    // std::cout << "[DEBUG] Gravity: " << dynamicsWorld_->getGravity().y() << std::endl;

Physics::~Physics() {
    delete dynamicsWorld_;
    delete solver_;
    delete dispatcher_;
    delete collisionConfig_;
    delete broadphase_;
}

void Physics::applyAcceleration(MC& mc) {
    auto vel = mc.getSpeedVector();
    mcRigidBody_->activate(true);
    mcRigidBody_->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

static int simulationSubsteps = 30; // Global or class-level variable

void Physics::setSubsteps(int steps) {
    simulationSubsteps = steps;
}
void Physics::update(MC& mc, float dt) {
    dynamicsWorld_->stepSimulation(dt,300);
    btTransform trans;
    mcRigidBody_->getMotionState()->getWorldTransform(trans);
    auto pos = trans.getOrigin();
    mc.setPosition({pos.x(), pos.y(), pos.z()});
    // constexpr btScalar mass = 0;
    // mcRigidBody_->applyCentralForce(btVector3(0, -gravity_ * mass, 0));
}

// void Physics::update(MC& mc, float dt) {
//     mcRigidBody_->activate(true);
//     mcRigidBody_->applyCentralForce(btVector3(0, -gravity_ * 300, 0)); // Force downward
//     dynamicsWorld_->stepSimulation(dt);
//
//     btTransform trans;
//     mcRigidBody_->getMotionState()->getWorldTransform(trans);
//     auto pos = trans.getOrigin();
//     mc.setPosition({pos.x(), pos.y(), pos.z()});
//
//     std::cout << "[DEBUG] Bike Y: " << pos.y() << std::endl;
// }

void Physics::setVelocity(const threepp::Vector3& vel) {
    velocity_ = vel;
    mcRigidBody_->activate(true);
    mcRigidBody_->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

const threepp::Vector3& Physics::getVelocity() const {
    return velocity_;
}

void Physics::resetVelocity() {
    velocity_ = {0, 0, 0};
    mcRigidBody_->activate(true);
    mcRigidBody_->setLinearVelocity(btVector3(0, 0, 0));
}

void Physics::setGravity(float g) {
    gravity_ = g;
    dynamicsWorld_->setGravity(btVector3(0, -gravity_, 0));
    mcRigidBody_->setGravity(btVector3(0, -gravity_, 0));
}

void Physics::setMass(MC& mc, float m) {
    btVector3 inertia(0, 0, 0);
    mcRigidBody_->getCollisionShape()->calculateLocalInertia(m, inertia);
    mcRigidBody_->setMassProps(m, inertia);
    mcRigidBody_->updateInertiaTensor();
}

void Physics::setFriction(float f) {
    mcRigidBody_->setFriction(f);
}

void Physics::setDamping(float linear, float angular) {
    mcRigidBody_->setDamping(linear, angular);
}

void Physics::resetRigidBodyPosition(const MC& mc) {
    btTransform resetTransform;
    resetTransform.setIdentity();
    resetTransform.setOrigin(btVector3(mc.getPosition().x, mc.getPosition().y, mc.getPosition().z));
    mcRigidBody_->setWorldTransform(resetTransform);
    mcRigidBody_->getMotionState()->setWorldTransform(resetTransform);
    mcRigidBody_->setLinearVelocity(btVector3(0, 0, 0)); // Stop movement
    mcRigidBody_->setAngularVelocity(btVector3(0, 0, 0)); // Stop rotation
}
