
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
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->HasMeshes()) {
        throw std::runtime_error("Failed to load OBJ: " + path);
    }

    btTriangleMesh* triMesh = new btTriangleMesh();

    for (unsigned m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];

        std::vector<btVector3> vertices;
        vertices.reserve(mesh->mNumVertices);
        for (unsigned i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D v = mesh->mVertices[i];
            vertices.emplace_back(v.x, v.y, v.z);
        }

        for (unsigned i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& face = mesh->mFaces[i];
            if (face.mNumIndices == 3) {
                triMesh->addTriangle(vertices[face.mIndices[0]],
                                      vertices[face.mIndices[1]],
                                      vertices[face.mIndices[2]]);
            }
        }
    }

    btBvhTriangleMeshShape* collisionShape = new btBvhTriangleMeshShape(triMesh, true);
    btDefaultMotionState* motionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

    btRigidBody::btRigidBodyConstructionInfo rbInfo(0, motionState, collisionShape, btVector3(0, 0, 0));
    return new btRigidBody(rbInfo);
}

Physics::Physics(float gravity, float groundLevel, MC& mc)
    : gravity_(gravity), groundLevel_(groundLevel), velocity_(0, 0, 0) {

    broadphase_ = new btDbvtBroadphase();
    collisionConfig_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(collisionConfig_);
    solver_ = new btSequentialImpulseConstraintSolver();
    dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collisionConfig_);
    dynamicsWorld_->setGravity(btVector3(0, -gravity_, 0));

    std::cout << "[INFO] Bullet physics initialized with gravity: " << gravity_
              << " and ground level: " << groundLevel_ << std::endl;

    // Add map collision
    try {
        btRigidBody* mapBody = createStaticCollisionShapeFromObj("models/ArgentinaTrack/source/PistaArgentina2.obj");
        dynamicsWorld_->addRigidBody(mapBody);
        std::cout << "[INFO] Map collision added from OBJ file." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to add map collision: " << e.what() << std::endl;
    }

    // Ground plane
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), groundLevel_);
    btDefaultMotionState* groundMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, groundLevel_, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    dynamicsWorld_->addRigidBody(new btRigidBody(groundCI));

    // Motorcycle rigid body
    btCollisionShape* mcShape = new btBoxShape(btVector3(1, 1, 1));
    btVector3 startPos(mc.getPosition().x, mc.getPosition().y, mc.getPosition().z);
    btDefaultMotionState* mcMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startPos));
    btScalar mass = 300;
    btVector3 inertia(0, 0, 0);
    mcShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo mcCI(mass, mcMotionState, mcShape, inertia);
    mcRigidBody_ = new btRigidBody(mcCI);
    mcRigidBody_->setFriction(0.5f);


    mcRigidBody_->setActivationState(DISABLE_DEACTIVATION); // Prevent sleeping
    mcRigidBody_->setGravity(btVector3(0, -gravity_, 0));   // Force gravity on this body
    std::cout << "[DEBUG] Gravity applied to bike: " << mcRigidBody_->getGravity().y() << std::endl;
    dynamicsWorld_->addRigidBody(mcRigidBody_);

    std::cout << "[DEBUG] Gravity: " << dynamicsWorld_->getGravity().y() << std::endl;

}

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

void Physics::update(MC& mc, float dt) {
    dynamicsWorld_->stepSimulation(dt);
    btTransform trans;
    mcRigidBody_->getMotionState()->getWorldTransform(trans);
    auto pos = trans.getOrigin();
    mc.setPosition({pos.x(), pos.y(), pos.z()});
    constexpr btScalar mass = 0;
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
