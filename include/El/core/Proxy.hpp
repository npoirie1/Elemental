/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef EL_CORE_PROXY_HPP
#define EL_CORE_PROXY_HPP

namespace El {

template<typename S,typename T,typename=EnableIf<CanCast<S,T>>>
class MatrixReadProxy 
{
private:
    bool locked_;

    Matrix<S>& orig_;
    Matrix<T>* prox_;

public:
    MatrixReadProxy( const Matrix<S>& A )
    : locked_(true),
      orig_(const_cast<Matrix<S>&>(A))
    {
        prox_ = new Matrix<T>;
        Copy( A, *prox_ );
    }

    MatrixReadProxy( Matrix<S>& A )
    : locked_(false),
      orig_(const_cast<Matrix<S>&>(A))
    { 
        prox_ = new Matrix<T>;
        Copy( A, *prox_ );
    }

    ~MatrixReadProxy() 
    { 
        delete prox_;
    }

    const Matrix<T>& GetLocked() const
    { return *prox_; }

    Matrix<T>& Get()
    {
        if( locked_ )
            LogicError("Attempted to extract mutable from immutable");
        return *prox_;
    }
};

template<typename T>
class MatrixReadProxy<T,T,void>
{
private:
    bool locked_;
    Matrix<T>& orig_;

public:
    MatrixReadProxy( const Matrix<T>& A )
    : locked_(true),
      orig_(const_cast<Matrix<T>&>(A))
    { }

    MatrixReadProxy( Matrix<T>& A )
    : locked_(false),
      orig_(const_cast<Matrix<T>&>(A))
    { }

    ~MatrixReadProxy() { }

    const Matrix<T>& GetLocked() const
    { return orig_; }

    Matrix<T>& Get()
    {
        if( locked_ )
            LogicError("Attempted to extract mutable from immutable");
        return orig_;
    }
};

template<typename S,typename T,typename=EnableIf<CanCast<T,S>>>
class MatrixWriteProxy 
{
private:
    Matrix<S>& orig_;
    Matrix<T>* prox_;

public:
    MatrixWriteProxy( Matrix<S>& A )
    : orig_(const_cast<Matrix<S>&>(A))
    { 
        prox_ = new Matrix<T>( A.Height(), A.Width() );
    }

    ~MatrixWriteProxy() 
    { 
        if( !uncaught_exception() )
            Copy( *prox_, orig_ );
        delete prox_;
    }

    const Matrix<T>& GetLocked() const
    { return *prox_; }

    Matrix<T>& Get()
    { return *prox_; }
};

template<typename T>
class MatrixWriteProxy<T,T,void>
{
private:
    Matrix<T>& orig_;

public:
    MatrixWriteProxy( Matrix<T>& A )
    : orig_(const_cast<Matrix<T>&>(A))
    { }

    ~MatrixWriteProxy() { }

    const Matrix<T>& GetLocked() const
    { return orig_; }

    Matrix<T>& Get()
    { return orig_; }
};

template<typename S,typename T,typename=EnableIf<CanBidirectionalCast<S,T>>>
class MatrixReadWriteProxy 
{
private:
    Matrix<S>& orig_;
    Matrix<T>* prox_;

public:
    MatrixReadWriteProxy( Matrix<S>& A )
    : orig_(const_cast<Matrix<S>&>(A))
    { 
        prox_ = new Matrix<T>(A);
    }

    ~MatrixReadWriteProxy() 
    { 
        if( !uncaught_exception() )
            Copy( *prox_, orig_ );
        delete prox_;
    }

    const Matrix<T>& GetLocked() const
    { return *prox_; }

    Matrix<T>& Get()
    { return *prox_; }
};

template<typename T>
class MatrixReadWriteProxy<T,T,void>
{
private:
    Matrix<T>& orig_;

public:
    MatrixReadWriteProxy( Matrix<T>& A )
    : orig_(const_cast<Matrix<T>&>(A))
    { }

    ~MatrixReadWriteProxy() { }

    const Matrix<T>& GetLocked() const
    { return orig_; }

    Matrix<T>& Get()
    { return orig_; }
};

struct ProxyCtrl 
{
    bool colConstrain, rowConstrain, rootConstrain;
    Int colAlign, rowAlign, root;
    Int blockHeight, blockWidth;
    Int colCut, rowCut;

    ProxyCtrl() 
    : colConstrain(false), rowConstrain(false), rootConstrain(false),
      colAlign(0), rowAlign(0), root(0), 
      blockHeight(DefaultBlockHeight()), blockWidth(DefaultBlockWidth()),
      colCut(0), rowCut(0)
    { }
};

struct ElementalProxyCtrl
{
    bool colConstrain, rowConstrain, rootConstrain;
    Int colAlign, rowAlign, root;

    ElementalProxyCtrl() 
    : colConstrain(false), rowConstrain(false), rootConstrain(false),
      colAlign(0), rowAlign(0), root(0) 
    { }
};

// TODO: Extend to support BLOCK distributions

template<typename S,typename T,Dist U=MC,Dist V=MR,
         typename=EnableIf<CanCast<S,T>>>
class DistMatrixReadProxy 
{
private:
    bool locked_;
    bool needDelete_;

    AbstractDistMatrix<S>& orig_;
    DistMatrix<T,U,V>* prox_;

public:
    // TODO: Add try-catch statements into constructors?

    DistMatrixReadProxy
    ( const AbstractDistMatrix<S>& A,
      const ElementalProxyCtrl& ctrl=ElementalProxyCtrl() )
    : locked_(true),
      orig_(const_cast<AbstractDistMatrix<S>&>(A))
    { 
        if( IsSame<S,T>::value && 
            A.ColDist() == U && A.RowDist() == V && A.Wrap() == ELEMENT)
        {
            const bool colMisalign = 
              ( ctrl.colConstrain && A.ColAlign() != ctrl.colAlign );
            const bool rowMisalign = 
              ( ctrl.rowConstrain && A.RowAlign() != ctrl.rowAlign );
            const bool rootMisalign = 
              ( ctrl.rootConstrain && A.Root() != ctrl.root );
            if( !colMisalign && !rowMisalign && !rootMisalign )
            {
                needDelete_ = false;
                auto ACast = dynamic_cast<const DistMatrix<T,U,V>*>(&A);
                // TODO: Ensure that ACast != null_ptr, though it should be
                //       guaranteed by the above if statement
                prox_ = const_cast<DistMatrix<T,U,V>*>(ACast);
                return;
            }
        }
        needDelete_ = true;
        prox_ = new DistMatrix<T,U,V>(A.Grid());
        if( ctrl.rootConstrain )
            prox_->SetRoot( ctrl.root );
        if( ctrl.colConstrain )
            prox_->AlignCols( ctrl.colAlign );    
        if( ctrl.rowConstrain )
            prox_->AlignRows( ctrl.rowAlign );
        Copy( A, *prox_ );
    }

    DistMatrixReadProxy
    ( AbstractDistMatrix<S>& A,
      const ElementalProxyCtrl& ctrl=ElementalProxyCtrl() )
    : locked_(false),
      orig_(const_cast<AbstractDistMatrix<S>&>(A))
    { 
        if( IsSame<S,T>::value &&
            A.ColDist() == U && A.RowDist() == V && A.Wrap() == ELEMENT )
        {
            const bool colMisalign = 
              ( ctrl.colConstrain && A.ColAlign() != ctrl.colAlign );
            const bool rowMisalign = 
              ( ctrl.rowConstrain && A.RowAlign() != ctrl.rowAlign );
            const bool rootMisalign = 
              ( ctrl.rootConstrain && A.Root() != ctrl.root );
            if( !colMisalign && !rowMisalign && !rootMisalign )
            {
                needDelete_ = false;
                prox_ = dynamic_cast<DistMatrix<T,U,V>*>(&A);
                // TODO: Ensure that prox_ != null_ptr, though it should be
                //       guaranteed by the above if statement
                return;
            }
        }
        needDelete_ = true;
        prox_ = new DistMatrix<T,U,V>(A.Grid());
        if( ctrl.rootConstrain )
            prox_->SetRoot( ctrl.root );
        if( ctrl.colConstrain )
            prox_->AlignCols( ctrl.colAlign );    
        if( ctrl.rowConstrain )
            prox_->AlignRows( ctrl.rowAlign );
        Copy( A, *prox_ );
    }

    ~DistMatrixReadProxy() 
    { 
        if( needDelete_ )
            delete prox_;
    }

    const DistMatrix<T,U,V>& GetLocked() const
    { return *prox_; }

    DistMatrix<T,U,V>& Get()
    {
        if( locked_ )
            LogicError("Attempted to extract mutable from immutable");
        return *prox_;
    }
};

template<typename S,typename T,Dist U=MC,Dist V=MR,
         typename=EnableIf<CanCast<T,S>>>
class DistMatrixWriteProxy 
{
private:
    bool needDelete_;
    bool needWriteCopy_;

    AbstractDistMatrix<S>& orig_;
    DistMatrix<T,U,V>* prox_;

public:
    // TODO: Add try-catch statements into constructors?
    DistMatrixWriteProxy
    ( AbstractDistMatrix<S>& A,
      const ElementalProxyCtrl& ctrl=ElementalProxyCtrl() )
    : orig_(const_cast<AbstractDistMatrix<S>&>(A))
    { 
        if( IsSame<S,T>::value &&
            A.ColDist() == U && A.RowDist() == V && A.Wrap() == ELEMENT )
        {
            const bool colMisalign = 
              ( ctrl.colConstrain && A.ColAlign() != ctrl.colAlign );
            const bool rowMisalign = 
              ( ctrl.rowConstrain && A.RowAlign() != ctrl.rowAlign );
            const bool rootMisalign = 
              ( ctrl.rootConstrain && A.Root() != ctrl.root );
            if( !colMisalign && !rowMisalign && !rootMisalign )
            {
                needDelete_ = false;
                needWriteCopy_ = false;
                prox_ = dynamic_cast<DistMatrix<T,U,V>*>(&A);
                // TODO: Ensure that prox_ != null_ptr, though it should be
                //       guaranteed by the above if statement
                return;
            }
        }
        needDelete_ = true;
        needWriteCopy_ = true;
        prox_ = new DistMatrix<T,U,V>(A.Grid());
        if( ctrl.rootConstrain )
            prox_->SetRoot( ctrl.root );
        if( ctrl.colConstrain )
            prox_->AlignCols( ctrl.colAlign );    
        if( ctrl.rowConstrain )
            prox_->AlignRows( ctrl.rowAlign );
        prox_->Resize( A.Height(), A.Width() );
    }

    ~DistMatrixWriteProxy() 
    { 
        if( needWriteCopy_ && !uncaught_exception() ) 
            Copy( *prox_, orig_ );
        if( needDelete_ )
            delete prox_;
    }

    const DistMatrix<T,U,V>& GetLocked() const
    { return *prox_; }

    DistMatrix<T,U,V>& Get()
    { return *prox_; }
};

template<typename S,typename T,Dist U=MC,Dist V=MR,
         typename=EnableIf<CanBidirectionalCast<S,T>>>
class DistMatrixReadWriteProxy 
{
private:
    bool needDelete_;
    bool needWriteCopy_;

    AbstractDistMatrix<S>& orig_;
    DistMatrix<T,U,V>* prox_;

public:
    // TODO: Add try-catch statements into constructors?
    DistMatrixReadWriteProxy
    ( AbstractDistMatrix<S>& A,
      const ElementalProxyCtrl& ctrl=ElementalProxyCtrl() )
    : orig_(const_cast<AbstractDistMatrix<S>&>(A))
    { 
        if( IsSame<S,T>::value &&
            A.ColDist() == U && A.RowDist() == V && A.Wrap() == ELEMENT )
        {
            const bool colMisalign = 
              ( ctrl.colConstrain && A.ColAlign() != ctrl.colAlign );
            const bool rowMisalign = 
              ( ctrl.rowConstrain && A.RowAlign() != ctrl.rowAlign );
            const bool rootMisalign = 
              ( ctrl.rootConstrain && A.Root() != ctrl.root );
            if( !colMisalign && !rowMisalign && !rootMisalign )
            {
                needDelete_ = false;
                needWriteCopy_ = false;
                prox_ = dynamic_cast<DistMatrix<T,U,V>*>(&A);
                // TODO: Ensure that prox_ != null_ptr, though it should be
                //       guaranteed by the above if statement
                return;
            }
        }
        needDelete_ = true;
        needWriteCopy_ = true;
        prox_ = new DistMatrix<T,U,V>(A.Grid());
        if( ctrl.rootConstrain )
            prox_->SetRoot( ctrl.root );
        if( ctrl.colConstrain )
            prox_->AlignCols( ctrl.colAlign );    
        if( ctrl.rowConstrain )
            prox_->AlignRows( ctrl.rowAlign );
        Copy( A, *prox_ );
    }

    ~DistMatrixReadWriteProxy() 
    { 
        if( needWriteCopy_ && !uncaught_exception() )
            Copy( *prox_, orig_ );
        if( needDelete_ )
            delete prox_;
    }

    const DistMatrix<T,U,V>& GetLocked() const
    { return *prox_; }

    DistMatrix<T,U,V>& Get()
    { return *prox_; }
};

} // namespace El

#endif // ifndef EL_CORE_PROXY_HPP
