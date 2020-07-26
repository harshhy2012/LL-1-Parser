# LL-1-Parser




A SAMPLE OUTPUT CAN BE SEEN BELOW :-

INPUT - 
  Grammar: 
    E ->  TR
    R ->  +TR
    R ->  #
    T ->  FS
    S ->  *FS
    S ->  #
    F ->  (E)
    F ->  i

  Terminal Symbols:
    +,  *,  (,  ),  i,  #
   
  Non-Terminal Symbols Leading to Epsilon(#):
    R,  S

OUTPUT - 

    FIRST(E) -> (, i, 
    FIRST(F) -> (, i,
    FIRST(R) -> #, +,
    FIRST(S) -> #, *,
    FIRST(T) -> (, i,

    FOLLOW(E) -> $, ),
    FOLLOW(F) -> $, ), *, +,
    FOLLOW(R) -> $, ),
    FOLLOW(S) -> $, ), +, 
    FOLLOW(T) -> $, ), +,


    FOLLOW : $ FOLLOW : ) FOLLOW : $ FOLLOW : ) FOLLOW : +
              |    +     |    *     |    (     |    )     |    i     |    $     |
    ------------------------------------------------------------------------------
        E     |          |          | E -> TR  |          | E -> TR  |          |
    ------------------------------------------------------------------------------
        R     | R -> +TR |          |          |  R -> #  |          |  R -> #  |
    ------------------------------------------------------------------------------
        T     |          |          | T -> FS  |          | T -> FS  |          |
    ------------------------------------------------------------------------------
        S     |  S -> #  | S -> *FS |          |  S -> #  |          |  S -> #  |
    ------------------------------------------------------------------------------
        F     |          |          | F -> (E) |          |  F -> i  |          |
    ------------------------------------------------------------------------------

    i+i*i+i
              STACK          |         STRING          |        OPERATION        
    ------------------------------------------------------------------------------
               $RT           |        i+i*i+i$         |        PUSH: RT
              $RSF           |        i+i*i+i$         |        PUSH: SF
              $RSi           |        i+i*i+i$         |         PUSH: i
                $RS          |         +i*i+i$         |    POP, PRODUCTION=#    
              $RT+           |         +i*i+i$         |        PUSH: RT+
              $RSF           |         i*i+i$          |        PUSH: SF
              $RT+           |         +i*i+i$         |        PUSH: RT+
              $RSF           |         i*i+i$          |        PUSH: SF
              $RSi           |         i*i+i$          |         PUSH: i
              $RSF*          |          *i+i$          |        PUSH: SF*
              $RSi           |          i+i$           |         PUSH: i
               $RS           |           +i$           |    POP, PRODUCTION=#
              $RT+           |           +i$           |        PUSH: RT+
              $RSi           |           i$            |         PUSH: i
              $RSF           |           i$            |        PUSH: SF
              $RSi           |           i$            |         PUSH: i
               $RS           |            $            |    POP, PRODUCTION=#
               $R            |            $            |    POP, PRODUCTION=#
