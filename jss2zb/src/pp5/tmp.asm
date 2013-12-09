	# standard Decaf preamble 
	  .text
	  .align 2
	  .globl main
  _QueueItem.Init:
	# BeginFunc 0
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	# data = data
	  lw $t2, 8($fp)	# fill data to $t2 from $fp+8
	  sw $t2, 8($fp)	# spill data from $t2 to $fp+8
	# next = next
	  lw $t2, 12($fp)	# fill next to $t2 from $fp+12
	  sw $t2, 12($fp)	# spill next from $t2 to $fp+12
	# prev = this
	  lw $t2, 4($fp)	# fill this to $t2 from $fp+4
	  sw $t2, 16($fp)	# spill prev from $t2 to $fp+16
	# prev = prev
	  lw $t2, 16($fp)	# fill prev to $t2 from $fp+16
	  sw $t2, 16($fp)	# spill prev from $t2 to $fp+16
	# next = this
	  lw $t2, 4($fp)	# fill this to $t2 from $fp+4
	  sw $t2, 12($fp)	# spill next from $t2 to $fp+12
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _QueueItem.GetData:
	# BeginFunc 4
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 4	# decrement sp to make space for locals/temps
	# _tmp0 = *(this + 4)
	  lw $t0, 4($fp)	# fill this to $t0 from $fp+4
	  lw $t2, 4($t0) 	# load with offset
	  sw $t2, -8($fp)	# spill _tmp0 from $t2 to $fp-8
	# Return _tmp0
	  lw $t2, -8($fp)	# fill _tmp0 to $t2 from $fp-8
	  move $v0, $t2		# assign return value into $v0
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _QueueItem.GetNext:
	# BeginFunc 4
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 4	# decrement sp to make space for locals/temps
	# _tmp1 = *(this + 8)
	  lw $t0, 4($fp)	# fill this to $t0 from $fp+4
	  lw $t2, 8($t0) 	# load with offset
	  sw $t2, -8($fp)	# spill _tmp1 from $t2 to $fp-8
	# Return _tmp1
	  lw $t2, -8($fp)	# fill _tmp1 to $t2 from $fp-8
	  move $v0, $t2		# assign return value into $v0
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _QueueItem.GetPrev:
	# BeginFunc 4
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 4	# decrement sp to make space for locals/temps
	# _tmp2 = *(this + 12)
	  lw $t0, 4($fp)	# fill this to $t0 from $fp+4
	  lw $t2, 12($t0) 	# load with offset
	  sw $t2, -8($fp)	# spill _tmp2 from $t2 to $fp-8
	# Return _tmp2
	  lw $t2, -8($fp)	# fill _tmp2 to $t2 from $fp-8
	  move $v0, $t2		# assign return value into $v0
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _QueueItem.SetNext:
	# BeginFunc 0
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	# *(this + 8) = n
	  lw $t0, 8($fp)	# fill n to $t0 from $fp+8
	  lw $t2, 4($fp)	# fill this to $t2 from $fp+4
	  sw $t0, 8($t2) 	# store with offset
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _QueueItem.SetPrev:
	# BeginFunc 0
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	# *(this + 12) = p
	  lw $t0, 8($fp)	# fill p to $t0 from $fp+8
	  lw $t2, 4($fp)	# fill this to $t2 from $fp+4
	  sw $t0, 12($t2) 	# store with offset
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# VTable for class QueueItem
	  .data
	  .align 2
	  QueueItem:		# label for class QueueItem vtable
	  .word _QueueItem.Init
	  .word _QueueItem.GetData
	  .word _QueueItem.GetNext
	  .word _QueueItem.GetPrev
	  .word _QueueItem.SetNext
	  .word _QueueItem.SetPrev
	  .text
  _Queue.Init:
	# BeginFunc 24
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 24	# decrement sp to make space for locals/temps
	# _tmp3 = 16
	  li $t2, 16		# load constant value 16 into $t2
	  sw $t2, -8($fp)	# spill _tmp3 from $t2 to $fp-8
	# PushParam _tmp3
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill _tmp3 to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp4 = LCall _Alloc
	  jal _Alloc         	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -12($fp)	# spill _tmp4 from $t2 to $fp-12
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp5 = QueueItem
	  la $t2, QueueItem	# load label
	  sw $t2, -16($fp)	# spill _tmp5 from $t2 to $fp-16
	# *(_tmp4) = _tmp5
	  lw $t0, -16($fp)	# fill _tmp5 to $t0 from $fp-16
	  lw $t2, -12($fp)	# fill _tmp4 to $t2 from $fp-12
	  sw $t0, 0($t2) 	# store with offset
	# *(this + 4) = _tmp4
	  lw $t0, -12($fp)	# fill _tmp4 to $t0 from $fp-12
	  lw $t2, 4($fp)	# fill this to $t2 from $fp+4
	  sw $t0, 4($t2) 	# store with offset
	# _tmp6 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -20($fp)	# spill _tmp6 from $t2 to $fp-20
	# _tmp7 = *(head)
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -24($fp)	# spill _tmp7 from $t2 to $fp-24
	# _tmp8 = *(_tmp7)
	  lw $t0, -24($fp)	# fill _tmp7 to $t0 from $fp-24
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -28($fp)	# spill _tmp8 from $t2 to $fp-28
	# PushParam head
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam head
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam _tmp6
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -20($fp)	# fill _tmp6 to $t0 from $fp-20
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam head
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# ACall _tmp8
	  lw $t0, -28($fp)	# fill _tmp8 to $t0 from $fp-28
	  jalr $t0            	# jump to function
	# PopParams 16
	  add $sp, $sp, 16	# pop params off stack
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _Queue.EnQueue:
	# BeginFunc 36
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 36	# decrement sp to make space for locals/temps
	# _tmp9 = 16
	  li $t2, 16		# load constant value 16 into $t2
	  sw $t2, -12($fp)	# spill _tmp9 from $t2 to $fp-12
	# PushParam _tmp9
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -12($fp)	# fill _tmp9 to $t0 from $fp-12
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp10 = LCall _Alloc
	  jal _Alloc         	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -16($fp)	# spill _tmp10 from $t2 to $fp-16
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp11 = QueueItem
	  la $t2, QueueItem	# load label
	  sw $t2, -20($fp)	# spill _tmp11 from $t2 to $fp-20
	# *(_tmp10) = _tmp11
	  lw $t0, -20($fp)	# fill _tmp11 to $t0 from $fp-20
	  lw $t2, -16($fp)	# fill _tmp10 to $t2 from $fp-16
	  sw $t0, 0($t2) 	# store with offset
	# temp = _tmp10
	  lw $t2, -16($fp)	# fill _tmp10 to $t2 from $fp-16
	  sw $t2, -8($fp)	# spill temp from $t2 to $fp-8
	# _tmp12 = *(head)
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -24($fp)	# spill _tmp12 from $t2 to $fp-24
	# _tmp13 = *(_tmp12 + 8)
	  lw $t0, -24($fp)	# fill _tmp12 to $t0 from $fp-24
	  lw $t2, 8($t0) 	# load with offset
	  sw $t2, -28($fp)	# spill _tmp13 from $t2 to $fp-28
	# PushParam head
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp14 = ACall _tmp13
	  lw $t0, -28($fp)	# fill _tmp13 to $t0 from $fp-28
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -32($fp)	# spill _tmp14 from $t2 to $fp-32
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp15 = *(temp)
	  lw $t0, -8($fp)	# fill temp to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -36($fp)	# spill _tmp15 from $t2 to $fp-36
	# _tmp16 = *(_tmp15)
	  lw $t0, -36($fp)	# fill _tmp15 to $t0 from $fp-36
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -40($fp)	# spill _tmp16 from $t2 to $fp-40
	# PushParam head
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam _tmp14
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -32($fp)	# fill _tmp14 to $t0 from $fp-32
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam i
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, 8($fp)	# fill i to $t0 from $fp+8
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam temp
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill temp to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# ACall _tmp16
	  lw $t0, -40($fp)	# fill _tmp16 to $t0 from $fp-40
	  jalr $t0            	# jump to function
	# PopParams 16
	  add $sp, $sp, 16	# pop params off stack
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
  _Queue.DeQueue:
	# BeginFunc 124
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 124	# decrement sp to make space for locals/temps
	# _tmp17 = *(head)
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -12($fp)	# spill _tmp17 from $t2 to $fp-12
	# _tmp18 = *(_tmp17 + 12)
	  lw $t0, -12($fp)	# fill _tmp17 to $t0 from $fp-12
	  lw $t2, 12($t0) 	# load with offset
	  sw $t2, -16($fp)	# spill _tmp18 from $t2 to $fp-16
	# PushParam head
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp19 = ACall _tmp18
	  lw $t0, -16($fp)	# fill _tmp18 to $t0 from $fp-16
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -20($fp)	# spill _tmp19 from $t2 to $fp-20
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp20 = *(this + 4)
	  lw $t0, 4($fp)	# fill this to $t0 from $fp+4
	  lw $t2, 4($t0) 	# load with offset
	  sw $t2, -24($fp)	# spill _tmp20 from $t2 to $fp-24
	# _tmp21 = _tmp19 == _tmp20
	  lw $t0, -20($fp)	# fill _tmp19 to $t0 from $fp-20
	  lw $t1, -24($fp)	# fill _tmp20 to $t1 from $fp-24
	  seq $t2, $t0, $t1	
	  sw $t2, -28($fp)	# spill _tmp21 from $t2 to $fp-28
	# IfZ _tmp21 Goto _L0
	  lw $t0, -28($fp)	# fill _tmp21 to $t0 from $fp-28
	  beqz $t0, _L0	# branch if _tmp21 is zero 
	# _tmp22 = "Queue Is Empty"
	  .data			# create string constant marked with label
	  _string1: .asciiz "Queue Is Empty"
	  .text
	  la $t2, _string1	# load label
	  sw $t2, -32($fp)	# spill _tmp22 from $t2 to $fp-32
	# PushParam _tmp22
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -32($fp)	# fill _tmp22 to $t0 from $fp-32
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintString
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp23 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -36($fp)	# spill _tmp23 from $t2 to $fp-36
	# Return _tmp23
	  lw $t2, -36($fp)	# fill _tmp23 to $t2 from $fp-36
	  move $v0, $t2		# assign return value into $v0
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# Goto _L1
	  b _L1		# unconditional branch
  _L0:
	# _tmp24 = *(head)
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -44($fp)	# spill _tmp24 from $t2 to $fp-44
	# _tmp25 = *(_tmp24 + 12)
	  lw $t0, -44($fp)	# fill _tmp24 to $t0 from $fp-44
	  lw $t2, 12($t0) 	# load with offset
	  sw $t2, -48($fp)	# spill _tmp25 from $t2 to $fp-48
	# PushParam head
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill head to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp26 = ACall _tmp25
	  lw $t0, -48($fp)	# fill _tmp25 to $t0 from $fp-48
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -52($fp)	# spill _tmp26 from $t2 to $fp-52
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# temp = _tmp26
	  lw $t2, -52($fp)	# fill _tmp26 to $t2 from $fp-52
	  sw $t2, -40($fp)	# spill temp from $t2 to $fp-40
	# _tmp27 = *(temp)
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -56($fp)	# spill _tmp27 from $t2 to $fp-56
	# _tmp28 = *(_tmp27 + 4)
	  lw $t0, -56($fp)	# fill _tmp27 to $t0 from $fp-56
	  lw $t2, 4($t0) 	# load with offset
	  sw $t2, -60($fp)	# spill _tmp28 from $t2 to $fp-60
	# PushParam temp
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp29 = ACall _tmp28
	  lw $t0, -60($fp)	# fill _tmp28 to $t0 from $fp-60
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -64($fp)	# spill _tmp29 from $t2 to $fp-64
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# val = _tmp29
	  lw $t2, -64($fp)	# fill _tmp29 to $t2 from $fp-64
	  sw $t2, -8($fp)	# spill val from $t2 to $fp-8
	# _tmp30 = *(temp)
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -68($fp)	# spill _tmp30 from $t2 to $fp-68
	# _tmp31 = *(_tmp30 + 8)
	  lw $t0, -68($fp)	# fill _tmp30 to $t0 from $fp-68
	  lw $t2, 8($t0) 	# load with offset
	  sw $t2, -72($fp)	# spill _tmp31 from $t2 to $fp-72
	# PushParam temp
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp32 = ACall _tmp31
	  lw $t0, -72($fp)	# fill _tmp31 to $t0 from $fp-72
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -76($fp)	# spill _tmp32 from $t2 to $fp-76
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp33 = *(temp)
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -80($fp)	# spill _tmp33 from $t2 to $fp-80
	# _tmp34 = *(_tmp33 + 12)
	  lw $t0, -80($fp)	# fill _tmp33 to $t0 from $fp-80
	  lw $t2, 12($t0) 	# load with offset
	  sw $t2, -84($fp)	# spill _tmp34 from $t2 to $fp-84
	# PushParam temp
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp35 = ACall _tmp34
	  lw $t0, -84($fp)	# fill _tmp34 to $t0 from $fp-84
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -88($fp)	# spill _tmp35 from $t2 to $fp-88
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp36 = *(_tmp35)
	  lw $t0, -88($fp)	# fill _tmp35 to $t0 from $fp-88
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -92($fp)	# spill _tmp36 from $t2 to $fp-92
	# _tmp37 = *(_tmp36 + 16)
	  lw $t0, -92($fp)	# fill _tmp36 to $t0 from $fp-92
	  lw $t2, 16($t0) 	# load with offset
	  sw $t2, -96($fp)	# spill _tmp37 from $t2 to $fp-96
	# PushParam _tmp32
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -76($fp)	# fill _tmp32 to $t0 from $fp-76
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam _tmp35
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -88($fp)	# fill _tmp35 to $t0 from $fp-88
	  sw $t0, 4($sp)	# copy param value to stack
	# ACall _tmp37
	  lw $t0, -96($fp)	# fill _tmp37 to $t0 from $fp-96
	  jalr $t0            	# jump to function
	# PopParams 8
	  add $sp, $sp, 8	# pop params off stack
	# _tmp38 = *(temp)
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -100($fp)	# spill _tmp38 from $t2 to $fp-100
	# _tmp39 = *(_tmp38 + 12)
	  lw $t0, -100($fp)	# fill _tmp38 to $t0 from $fp-100
	  lw $t2, 12($t0) 	# load with offset
	  sw $t2, -104($fp)	# spill _tmp39 from $t2 to $fp-104
	# PushParam temp
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp40 = ACall _tmp39
	  lw $t0, -104($fp)	# fill _tmp39 to $t0 from $fp-104
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -108($fp)	# spill _tmp40 from $t2 to $fp-108
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp41 = *(temp)
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -112($fp)	# spill _tmp41 from $t2 to $fp-112
	# _tmp42 = *(_tmp41 + 8)
	  lw $t0, -112($fp)	# fill _tmp41 to $t0 from $fp-112
	  lw $t2, 8($t0) 	# load with offset
	  sw $t2, -116($fp)	# spill _tmp42 from $t2 to $fp-116
	# PushParam temp
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -40($fp)	# fill temp to $t0 from $fp-40
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp43 = ACall _tmp42
	  lw $t0, -116($fp)	# fill _tmp42 to $t0 from $fp-116
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -120($fp)	# spill _tmp43 from $t2 to $fp-120
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp44 = *(_tmp43)
	  lw $t0, -120($fp)	# fill _tmp43 to $t0 from $fp-120
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -124($fp)	# spill _tmp44 from $t2 to $fp-124
	# _tmp45 = *(_tmp44 + 20)
	  lw $t0, -124($fp)	# fill _tmp44 to $t0 from $fp-124
	  lw $t2, 20($t0) 	# load with offset
	  sw $t2, -128($fp)	# spill _tmp45 from $t2 to $fp-128
	# PushParam _tmp40
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -108($fp)	# fill _tmp40 to $t0 from $fp-108
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam _tmp43
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -120($fp)	# fill _tmp43 to $t0 from $fp-120
	  sw $t0, 4($sp)	# copy param value to stack
	# ACall _tmp45
	  lw $t0, -128($fp)	# fill _tmp45 to $t0 from $fp-128
	  jalr $t0            	# jump to function
	# PopParams 8
	  add $sp, $sp, 8	# pop params off stack
  _L1:
	# Return val
	  lw $t2, -8($fp)	# fill val to $t2 from $fp-8
	  move $v0, $t2		# assign return value into $v0
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
	# VTable for class Queue
	  .data
	  .align 2
	  Queue:		# label for class Queue vtable
	  .word _Queue.Init
	  .word _Queue.EnQueue
	  .word _Queue.DeQueue
	  .text
  main:
	# BeginFunc 180
	  subu $sp, $sp, 8	# decrement sp to make space to save ra, fp
	  sw $fp, 8($sp)	# save fp
	  sw $ra, 4($sp)	# save ra
	  addiu $fp, $sp, 8	# set up new fp
	  subu $sp, $sp, 180	# decrement sp to make space for locals/temps
	# _tmp46 = 8
	  li $t2, 8		# load constant value 8 into $t2
	  sw $t2, -16($fp)	# spill _tmp46 from $t2 to $fp-16
	# PushParam _tmp46
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -16($fp)	# fill _tmp46 to $t0 from $fp-16
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp47 = LCall _Alloc
	  jal _Alloc         	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -20($fp)	# spill _tmp47 from $t2 to $fp-20
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp48 = Queue
	  la $t2, Queue	# load label
	  sw $t2, -24($fp)	# spill _tmp48 from $t2 to $fp-24
	# *(_tmp47) = _tmp48
	  lw $t0, -24($fp)	# fill _tmp48 to $t0 from $fp-24
	  lw $t2, -20($fp)	# fill _tmp47 to $t2 from $fp-20
	  sw $t0, 0($t2) 	# store with offset
	# q = _tmp47
	  lw $t2, -20($fp)	# fill _tmp47 to $t2 from $fp-20
	  sw $t2, -8($fp)	# spill q from $t2 to $fp-8
	# _tmp49 = *(q)
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -28($fp)	# spill _tmp49 from $t2 to $fp-28
	# _tmp50 = *(_tmp49)
	  lw $t0, -28($fp)	# fill _tmp49 to $t0 from $fp-28
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -32($fp)	# spill _tmp50 from $t2 to $fp-32
	# PushParam q
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# ACall _tmp50
	  lw $t0, -32($fp)	# fill _tmp50 to $t0 from $fp-32
	  jalr $t0            	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp51 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -36($fp)	# spill _tmp51 from $t2 to $fp-36
	# i = _tmp51
	  lw $t2, -36($fp)	# fill _tmp51 to $t2 from $fp-36
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
  _L2:
	# _tmp52 = 10
	  li $t2, 10		# load constant value 10 into $t2
	  sw $t2, -40($fp)	# spill _tmp52 from $t2 to $fp-40
	# _tmp53 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -44($fp)	# spill _tmp53 from $t2 to $fp-44
	# _tmp54 = i == _tmp52
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -40($fp)	# fill _tmp52 to $t1 from $fp-40
	  seq $t2, $t0, $t1	
	  sw $t2, -48($fp)	# spill _tmp54 from $t2 to $fp-48
	# IfZ _tmp54 Goto _L4
	  lw $t0, -48($fp)	# fill _tmp54 to $t0 from $fp-48
	  beqz $t0, _L4	# branch if _tmp54 is zero 
	# IfZ _tmp54 Goto _L3
	  lw $t0, -48($fp)	# fill _tmp54 to $t0 from $fp-48
	  beqz $t0, _L3	# branch if _tmp54 is zero 
	# _tmp55 = *(q)
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -52($fp)	# spill _tmp55 from $t2 to $fp-52
	# _tmp56 = *(_tmp55 + 4)
	  lw $t0, -52($fp)	# fill _tmp55 to $t0 from $fp-52
	  lw $t2, 4($t0) 	# load with offset
	  sw $t2, -56($fp)	# spill _tmp56 from $t2 to $fp-56
	# PushParam i
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam q
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# ACall _tmp56
	  lw $t0, -56($fp)	# fill _tmp56 to $t0 from $fp-56
	  jalr $t0            	# jump to function
	# PopParams 8
	  add $sp, $sp, 8	# pop params off stack
	# _tmp57 = 1
	  li $t2, 1		# load constant value 1 into $t2
	  sw $t2, -60($fp)	# spill _tmp57 from $t2 to $fp-60
	# _tmp58 = i + _tmp57
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -60($fp)	# fill _tmp57 to $t1 from $fp-60
	  add $t2, $t0, $t1	
	  sw $t2, -64($fp)	# spill _tmp58 from $t2 to $fp-64
	# i = _tmp58
	  lw $t2, -64($fp)	# fill _tmp58 to $t2 from $fp-64
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
	# Goto _L2
	  b _L2		# unconditional branch
  _L3:
	# _tmp59 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -68($fp)	# spill _tmp59 from $t2 to $fp-68
	# i = _tmp59
	  lw $t2, -68($fp)	# fill _tmp59 to $t2 from $fp-68
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
  _L5:
	# _tmp60 = 4
	  li $t2, 4		# load constant value 4 into $t2
	  sw $t2, -72($fp)	# spill _tmp60 from $t2 to $fp-72
	# _tmp61 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -76($fp)	# spill _tmp61 from $t2 to $fp-76
	# _tmp62 = i == _tmp60
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -72($fp)	# fill _tmp60 to $t1 from $fp-72
	  seq $t2, $t0, $t1	
	  sw $t2, -80($fp)	# spill _tmp62 from $t2 to $fp-80
	# IfZ _tmp62 Goto _L7
	  lw $t0, -80($fp)	# fill _tmp62 to $t0 from $fp-80
	  beqz $t0, _L7	# branch if _tmp62 is zero 
	# IfZ _tmp62 Goto _L6
	  lw $t0, -80($fp)	# fill _tmp62 to $t0 from $fp-80
	  beqz $t0, _L6	# branch if _tmp62 is zero 
	# _tmp63 = *(q)
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -84($fp)	# spill _tmp63 from $t2 to $fp-84
	# _tmp64 = *(_tmp63 + 8)
	  lw $t0, -84($fp)	# fill _tmp63 to $t0 from $fp-84
	  lw $t2, 8($t0) 	# load with offset
	  sw $t2, -88($fp)	# spill _tmp64 from $t2 to $fp-88
	# PushParam q
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp65 = ACall _tmp64
	  lw $t0, -88($fp)	# fill _tmp64 to $t0 from $fp-88
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -92($fp)	# spill _tmp65 from $t2 to $fp-92
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# PushParam _tmp65
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -92($fp)	# fill _tmp65 to $t0 from $fp-92
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintInt
	  jal _PrintInt      	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp66 = " "
	  .data			# create string constant marked with label
	  _string2: .asciiz " "
	  .text
	  la $t2, _string2	# load label
	  sw $t2, -96($fp)	# spill _tmp66 from $t2 to $fp-96
	# PushParam _tmp66
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -96($fp)	# fill _tmp66 to $t0 from $fp-96
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintString
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp67 = 1
	  li $t2, 1		# load constant value 1 into $t2
	  sw $t2, -100($fp)	# spill _tmp67 from $t2 to $fp-100
	# _tmp68 = i + _tmp67
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -100($fp)	# fill _tmp67 to $t1 from $fp-100
	  add $t2, $t0, $t1	
	  sw $t2, -104($fp)	# spill _tmp68 from $t2 to $fp-104
	# i = _tmp68
	  lw $t2, -104($fp)	# fill _tmp68 to $t2 from $fp-104
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
	# Goto _L5
	  b _L5		# unconditional branch
  _L6:
	# _tmp69 = "\n"
	  .data			# create string constant marked with label
	  _string3: .asciiz "\n"
	  .text
	  la $t2, _string3	# load label
	  sw $t2, -108($fp)	# spill _tmp69 from $t2 to $fp-108
	# PushParam _tmp69
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -108($fp)	# fill _tmp69 to $t0 from $fp-108
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintString
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp70 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -112($fp)	# spill _tmp70 from $t2 to $fp-112
	# i = _tmp70
	  lw $t2, -112($fp)	# fill _tmp70 to $t2 from $fp-112
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
  _L8:
	# _tmp71 = 10
	  li $t2, 10		# load constant value 10 into $t2
	  sw $t2, -116($fp)	# spill _tmp71 from $t2 to $fp-116
	# _tmp72 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -120($fp)	# spill _tmp72 from $t2 to $fp-120
	# _tmp73 = i == _tmp71
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -116($fp)	# fill _tmp71 to $t1 from $fp-116
	  seq $t2, $t0, $t1	
	  sw $t2, -124($fp)	# spill _tmp73 from $t2 to $fp-124
	# IfZ _tmp73 Goto _L10
	  lw $t0, -124($fp)	# fill _tmp73 to $t0 from $fp-124
	  beqz $t0, _L10	# branch if _tmp73 is zero 
	# IfZ _tmp73 Goto _L9
	  lw $t0, -124($fp)	# fill _tmp73 to $t0 from $fp-124
	  beqz $t0, _L9	# branch if _tmp73 is zero 
	# _tmp74 = *(q)
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -128($fp)	# spill _tmp74 from $t2 to $fp-128
	# _tmp75 = *(_tmp74 + 4)
	  lw $t0, -128($fp)	# fill _tmp74 to $t0 from $fp-128
	  lw $t2, 4($t0) 	# load with offset
	  sw $t2, -132($fp)	# spill _tmp75 from $t2 to $fp-132
	# PushParam i
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  sw $t0, 4($sp)	# copy param value to stack
	# PushParam q
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# ACall _tmp75
	  lw $t0, -132($fp)	# fill _tmp75 to $t0 from $fp-132
	  jalr $t0            	# jump to function
	# PopParams 8
	  add $sp, $sp, 8	# pop params off stack
	# _tmp76 = 1
	  li $t2, 1		# load constant value 1 into $t2
	  sw $t2, -136($fp)	# spill _tmp76 from $t2 to $fp-136
	# _tmp77 = i + _tmp76
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -136($fp)	# fill _tmp76 to $t1 from $fp-136
	  add $t2, $t0, $t1	
	  sw $t2, -140($fp)	# spill _tmp77 from $t2 to $fp-140
	# i = _tmp77
	  lw $t2, -140($fp)	# fill _tmp77 to $t2 from $fp-140
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
	# Goto _L8
	  b _L8		# unconditional branch
  _L9:
	# _tmp78 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -144($fp)	# spill _tmp78 from $t2 to $fp-144
	# i = _tmp78
	  lw $t2, -144($fp)	# fill _tmp78 to $t2 from $fp-144
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
  _L11:
	# _tmp79 = 17
	  li $t2, 17		# load constant value 17 into $t2
	  sw $t2, -148($fp)	# spill _tmp79 from $t2 to $fp-148
	# _tmp80 = 0
	  li $t2, 0		# load constant value 0 into $t2
	  sw $t2, -152($fp)	# spill _tmp80 from $t2 to $fp-152
	# _tmp81 = i == _tmp79
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -148($fp)	# fill _tmp79 to $t1 from $fp-148
	  seq $t2, $t0, $t1	
	  sw $t2, -156($fp)	# spill _tmp81 from $t2 to $fp-156
	# IfZ _tmp81 Goto _L13
	  lw $t0, -156($fp)	# fill _tmp81 to $t0 from $fp-156
	  beqz $t0, _L13	# branch if _tmp81 is zero 
	# IfZ _tmp81 Goto _L12
	  lw $t0, -156($fp)	# fill _tmp81 to $t0 from $fp-156
	  beqz $t0, _L12	# branch if _tmp81 is zero 
	# _tmp82 = *(q)
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  lw $t2, 0($t0) 	# load with offset
	  sw $t2, -160($fp)	# spill _tmp82 from $t2 to $fp-160
	# _tmp83 = *(_tmp82 + 8)
	  lw $t0, -160($fp)	# fill _tmp82 to $t0 from $fp-160
	  lw $t2, 8($t0) 	# load with offset
	  sw $t2, -164($fp)	# spill _tmp83 from $t2 to $fp-164
	# PushParam q
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -8($fp)	# fill q to $t0 from $fp-8
	  sw $t0, 4($sp)	# copy param value to stack
	# _tmp84 = ACall _tmp83
	  lw $t0, -164($fp)	# fill _tmp83 to $t0 from $fp-164
	  jalr $t0            	# jump to function
	  move $t2, $v0		# copy function return value from $v0
	  sw $t2, -168($fp)	# spill _tmp84 from $t2 to $fp-168
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# PushParam _tmp84
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -168($fp)	# fill _tmp84 to $t0 from $fp-168
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintInt
	  jal _PrintInt      	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp85 = " "
	  .data			# create string constant marked with label
	  _string4: .asciiz " "
	  .text
	  la $t2, _string4	# load label
	  sw $t2, -172($fp)	# spill _tmp85 from $t2 to $fp-172
	# PushParam _tmp85
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -172($fp)	# fill _tmp85 to $t0 from $fp-172
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintString
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# _tmp86 = 1
	  li $t2, 1		# load constant value 1 into $t2
	  sw $t2, -176($fp)	# spill _tmp86 from $t2 to $fp-176
	# _tmp87 = i + _tmp86
	  lw $t0, -12($fp)	# fill i to $t0 from $fp-12
	  lw $t1, -176($fp)	# fill _tmp86 to $t1 from $fp-176
	  add $t2, $t0, $t1	
	  sw $t2, -180($fp)	# spill _tmp87 from $t2 to $fp-180
	# i = _tmp87
	  lw $t2, -180($fp)	# fill _tmp87 to $t2 from $fp-180
	  sw $t2, -12($fp)	# spill i from $t2 to $fp-12
	# Goto _L11
	  b _L11		# unconditional branch
  _L12:
	# _tmp88 = "\n"
	  .data			# create string constant marked with label
	  _string5: .asciiz "\n"
	  .text
	  la $t2, _string5	# load label
	  sw $t2, -184($fp)	# spill _tmp88 from $t2 to $fp-184
	# PushParam _tmp88
	  subu $sp, $sp, 4	# decrement sp to make space for param
	  lw $t0, -184($fp)	# fill _tmp88 to $t0 from $fp-184
	  sw $t0, 4($sp)	# copy param value to stack
	# LCall _PrintString
	  jal _PrintString   	# jump to function
	# PopParams 4
	  add $sp, $sp, 4	# pop params off stack
	# EndFunc
	# (below handles reaching end of fn body with no explicit return)
	  move $sp, $fp		# pop callee frame off stack
	  lw $ra, -4($fp)	# restore saved ra
	  lw $fp, 0($fp)	# restore saved fp
	  jr $ra		# return from function
_PrintInt:
        subu $sp, $sp, 8
        sw $fp, 8($sp)
        sw $ra, 4($sp)
        addiu $fp, $sp, 8
        li   $v0, 1
        lw   $a0, 4($fp)
        syscall
        move $sp, $fp
        lw $ra, -4($fp)
        lw $fp, 0($fp)
        jr $ra
                                
_PrintString:
        subu $sp, $sp, 8
        sw $fp, 8($sp)
        sw $ra, 4($sp)
        addiu $fp, $sp, 8
        li   $v0, 4
        lw $a0, 4($fp)
        syscall
        move $sp, $fp
        lw $ra, -4($fp)
        lw $fp, 0($fp)
        jr $ra
        
_PrintBool:
	subu $sp, $sp, 8
	sw $fp, 8($sp)
	sw $ra, 4($sp)
        addiu $fp, $sp, 8
	lw $t1, 4($fp)
	blez $t1, fbr
	li   $v0, 4		# system call for print_str
	la   $a0, TRUE		# address of str to print
	syscall			
	b end
fbr:	li   $v0, 4		# system call for print_str
	la   $a0, FALSE		# address of str to print
	syscall				
end:	move $sp, $fp
	lw $ra, -4($fp)
	lw $fp, 0($fp)
	jr $ra

_Alloc:
        subu $sp, $sp, 8
        sw $fp, 8($sp)
        sw $ra, 4($sp)
        addiu $fp, $sp, 8
        li   $v0, 9
        lw $a0, 4($fp)
	syscall
        move $sp, $fp
        lw $ra, -4($fp)
        lw $fp, 0($fp) 
        jr $ra


_StringEqual:
	subu $sp, $sp, 8      # decrement sp to make space to save ra, fp
	sw $fp, 8($sp)        # save fp
	sw $ra, 4($sp)        # save ra
	addiu $fp, $sp, 8     # set up new fp
	subu $sp, $sp, 4      # decrement sp to make space for locals/temps

	li $v0,0

	#Determine length string 1
	lw $t0, 4($fp)       
	li $t3,0
bloop1: 
	lb $t5, ($t0) 
	beqz $t5, eloop1	
	addi $t0, 1
	addi $t3, 1
	b bloop1
eloop1:

	#Determine length string 2
	lw $t1, 8($fp)
	li $t4,0
bloop2: 
	lb $t5, ($t1) 
	beqz $t5, eloop2	
	addi $t1, 1
	addi $t4, 1
	b bloop2
eloop2:
	bne $t3,$t4,end1       #Check String Lengths Same

	lw $t0, 4($fp)       
	lw $t1, 8($fp)
	li $t3, 0     		
bloop3:	
	lb $t5, ($t0) 
	lb $t6, ($t1) 
	bne $t5, $t6, end1
	addi $t3, 1
	addi $t0, 1
	addi $t1, 1
	bne $t3,$t4,bloop3
eloop3:	li $v0,1

end1:	move $sp, $fp         # pop callee frame off stack
	lw $ra, -4($fp)       # restore saved ra
	lw $fp, 0($fp)        # restore saved fp
	jr $ra                # return from function

_Halt:
        li $v0, 10
        syscall

_ReadInteger:
	subu $sp, $sp, 8      # decrement sp to make space to save ra, fp
	sw $fp, 8($sp)        # save fp
	sw $ra, 4($sp)        # save ra
	addiu $fp, $sp, 8     # set up new fp
	subu $sp, $sp, 4      # decrement sp to make space for locals/temps
	li $v0, 5
	syscall
	move $sp, $fp         # pop callee frame off stack
	lw $ra, -4($fp)       # restore saved ra
	lw $fp, 0($fp)        # restore saved fp
	jr $ra
        

_ReadLine:
	subu $sp, $sp, 8      # decrement sp to make space to save ra, fp
	sw $fp, 8($sp)        # save fp
	sw $ra, 4($sp)        # save ra
	addiu $fp, $sp, 8     # set up new fp
	subu $sp, $sp, 4      # decrement sp to make space for locals/temps
	# allocate space to store memory
	li $a0, 128           # request 128 bytes
	li $v0, 9	      # syscall "sbrk" for memory allocation
	syscall               # do the system call
	# read in the new line
	li $a1, 128	      # size of the buffer
	#la $a0, SPACE        
	move $a0, $v0	      # location of the buffer	
	li $v0, 8 
	syscall

	#la $t1, SPACE  
	move $t1, $a0
bloop4: 
	lb $t5, ($t1) 
	beqz $t5, eloop4	
	addi $t1, 1
	b bloop4
eloop4:
	addi $t1,-1
	li $t6,0
	sb $t6, ($t1)

	#la $v0, SPACE
	move $v0, $a0	      # save buffer location to v0 as return value	
	move $sp, $fp         # pop callee frame off stack
	lw $ra, -4($fp)       # restore saved ra
	lw $fp, 0($fp)        # restore saved fp
	jr $ra
	

	.data
TRUE:.asciiz "true"
FALSE:.asciiz "false"
SPACE:.asciiz "Making Space For Inputed Values Is Fun."