This is a reduced version of FreeRTOSV6.1.1. This file describes the alterations being made.

All the demos have been removed.

Almost all the ports have been removed, except the following:
    ARM-CM3
    MIPS4

The following modifications have been made:

-) Introducing the newlib's reent system into task.c
-) Fixing heap_3.c to add missing calls

--- tasks.c	2011-01-14 20:14:24.000000000 -0800
+++ tasks.c	2011-01-22 22:33:27.000000000 -0800
@@ -114,6 +114,10 @@
 		unsigned long ulRunTimeCounter;		/*< Used for calculating how much CPU time each task is utilising. */
 	#endif
 
+	#if ( configUSE_NEWLIB_REENTRANT == 1 )
+		struct _reent reent;
+	#endif
+
 } tskTCB;
 
 
@@ -1635,6 +1639,10 @@
 	same priority get an equal share of the processor time. */
 	listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists[ uxTopReadyPriority ] ) );
 
+	#if ( configUSE_NEWLIB_REENTRANT == 1 )
+		_impure_ptr = &(pxCurrentTCB->reent);
+	#endif
+
 	traceTASK_SWITCHED_IN();
 	vWriteTraceToBuffer();
 }
@@ -1952,6 +1960,10 @@
 		( void ) usStackDepth;
 	}
 	#endif
+
+	#if ( configUSE_NEWLIB_REENTRANT == 1 )
+		_REENT_INIT_PTR((&(pxTCB->reent)));
+	#endif
 }
 /*-----------------------------------------------------------*/
 
--- heap_3.c	2011-01-14 20:14:24.000000000 -0800
+++ heap_3.c	2011-01-22 23:44:03.000000000 -0800
@@ -99,6 +99,7 @@
 	
 	return pvReturn;
 }
+
 /*-----------------------------------------------------------*/
 
 void vPortFree( void *pv )
@@ -113,5 +114,18 @@
 	}
 }
 
+/*-----------------------------------------------------------*/
+
+void vPortInitialiseBlocks( void )
+{
+	/* Not necessary */
+}
+
+/*-----------------------------------------------------------*/
+
+size_t xPortGetFreeHeapSize( void )
+{
+	return 0; /* Not necessary anyway */
+}
 
 
