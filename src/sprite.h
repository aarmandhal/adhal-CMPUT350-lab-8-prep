#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    Sprite(const sf::Texture& texture) {
        //
        mTexture = &texture;
        mMinPointTex = {0.0f, 0.0f};
        mMaxPointTex = static_cast<sf::Vector2f>(texture.getSize());
        mOrigin = {0.0f, 0.0f};
        mTransform = BasicTransform();
        mVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        mVertexArray.resize(6);
        recomputeVertexArray();
    }

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint) {
        // Copy tex subrect and set origin to center of the subrect
        mTexture = &texture;
        // texMinPoint is the min point of the subrect
        mMinPointTex = texMinPoint;
        // texMaxPoint is the max point of the subrect
        mMaxPointTex = texMaxPoint;
        // origin is the center of the subrect
        mOrigin = {0.0f, 0.0f};
        // mTransform is initialized to the identity transform
        mTransform = BasicTransform();
        // Set the primitive type to triangles and resize the vertex array
        mVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        mVertexArray.resize(6);
        // Recompute the vertex array
        recomputeVertexArray();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // if texture is null, do nothing
        if (mTexture == nullptr) {
            return;
        }
        // recompute the vertex array to update positions and texture coordinates
        recomputeVertexArray();  // transform/origin may have changed since last frame
        states.texture = mTexture;
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    // Unit quad [0,1]^2, split along the anti-diagonal (TR <-> BL) into
    // two CCW triangles: {TL, BL, TR} (top-left half), {TR, BL, BR} (bottom-right half).
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {
        sf::Vector2f{0.f, 0.f},  // TL ┐
        sf::Vector2f{0.f, 1.f},  // BL ├ triangle 1 (top-left)
        sf::Vector2f{1.f, 0.f},  // TR ┘
        sf::Vector2f{1.f, 0.f},  // TR ┐
        sf::Vector2f{0.f, 1.f},  // BL ├ triangle 2 (bottom-right)
        sf::Vector2f{1.f, 1.f},  // BR ┘
    };

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    void recomputeVertexArray() const {
        // if texture is null, do nothing
        if (mTexture == nullptr) {
            return;
        }

        // subrect size, texture-pixel units
        sf::Vector2f texSize = mMaxPointTex - mMinPointTex;

        for (int i = 0; i < 6; i++) {
            // in [0,1]^2
            sf::Vector2f local = DEFAULT_VERTICES[i];

            // texture coords: blend between mMinPointTex and mMaxPointTex
            sf::Vector2f texCoord = mMinPointTex + local * texSize;

            // model-space position: quad spans [0, texSize], shifted so mOrigin
            // sits at local (0,0) before the transform is applied
            sf::Vector2f modelPos = local * texSize - mOrigin;

            // Apply the transform and set the texture coordinates
            mVertexArray[i].position = mTransform.applyPoint(modelPos);
            mVertexArray[i].texCoords = texCoord;
        }
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif