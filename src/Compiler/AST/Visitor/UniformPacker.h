/*
 * UniformPacker.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2019 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_UNIFORM_PACKER_H
#define XSC_UNIFORM_PACKER_H


#include "Visitor.h"
#include "TypeDenoter.h"
#include <string>


namespace Xsc
{


/*
Uniform packer is not a visitor in the conventional sense.
It only itertates over all global statements and moves all uniform declarations into a single uniform buffer.
*/
class UniformPacker
{

    public:

        struct CbufferAttributes
        {
            //NOTE: workaround bug in Clang and GCC where the default constructor struggles with default arguments of braced initializers
            #if defined __clang__ || defined __GNUC__
            inline CbufferAttributes() {}
            CbufferAttributes(const CbufferAttributes&) = default;
            CbufferAttributes& operator = (const CbufferAttributes&) = default;
            #endif

            // Zero-based binding slot, where a negative number indicates to ignore this value.
            int         bindingSlot = 0;

            // Name of the uniform buffer object.
            std::string name        = "xsp_cbuffer";
        };

        // Converts the program by moving all global uniform declarations into a single uniform buffer.
        void Convert(Program& program, const CbufferAttributes& cbufferAttribs = {}, bool onlyReachableStmts = true);

    private:

        void MakeUniformBuffer();
        void AppendUniform(const VarDeclStmtPtr& varDeclStmt);

        bool CanConvertUniformWithTypeDenoter(const TypeDenoter& typeDen) const;

    private:

        CbufferAttributes       cbufferAttribs_;

        UniformBufferDeclPtr    uniformBufferDecl_;
        BasicDeclStmtPtr        declStmt_;

};


} // /namespace Xsc


#endif



// ================================================================================
